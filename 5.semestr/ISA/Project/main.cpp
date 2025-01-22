/*
ISA project - PCAP NetFlow v5 exporter
Author: Michálek Kryštof (xmicha94) 
*/

#include <iostream>
#include <unistd.h>
#include <pcap.h>
#include <map>
#include <vector>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <chrono>
#include "packet.h"
#include "flow.h"
#include "netflowV5.h"

const int MAX_FLOWS = 30;
int flow_sequence = 0;

int main(int argc, char* argv[]) {
    std::string host;
    std::string port;
    std::string pcap_file_path;

    // Default timeouts
    int active_timeout = 60;
    int inactive_timeout = 60;

    // Timestamp of the export device boot
    auto timestamp_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // Parse -i and -a arguments
    int opt;
    while ((opt = getopt(argc, argv, "a:i:")) != -1) {
        switch (opt) {
            case 'a':
                active_timeout = std::stoi(optarg); 
                break;
            case 'i':
                inactive_timeout = std::stoi(optarg);
                break;
            default: 
                std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
                return 1;
        }
    }

    // Parse other arguments
    for (int i = optind; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find(':') != std::string::npos) {
            size_t colon_pos = arg.find(':');
            host = arg.substr(0, colon_pos);
            port = arg.substr(colon_pos + 1);
        } else {
            if(pcap_file_path == "") {
                pcap_file_path = arg;

            }
            else {
                std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
                return 1;
            }
        }
    }

    // Validate required parameters
    if (host.empty() || port.empty() || pcap_file_path.empty()) {
        std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
        return 1;
    }

    // open PCAP file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = pcap_open_offline(pcap_file_path.c_str(), errbuf);
    if (pcap == nullptr) {
        std::cerr << "Error opening file: " << errbuf << std::endl;
        return 1;
    }

    // Variables for packet
    const u_char* packet_data;
    struct pcap_pkthdr header;

    // List of active flows and buffer to export flows
    std::vector<Flow> active_flows;
    std::vector<netflowV5Record> flow_buffer;

    // Loop through all packets in the file
    while ((packet_data = pcap_next(pcap, &header)) != nullptr) {
        struct ip* ip_header = (struct ip*)(packet_data + 14);
        if (ip_header->ip_p == IPPROTO_TCP) {
            struct tcphdr* tcp_header = (struct tcphdr*)(packet_data + 14 + (ip_header->ip_hl * 4));

            // Prepare data for the packet class object
            uint32_t src_ip = ip_header->ip_src.s_addr;
            uint32_t dst_ip = ip_header->ip_dst.s_addr;
            uint16_t src_port = tcp_header->source;
            uint16_t dst_port = tcp_header->dest;
            uint64_t timestamp = header.ts.tv_sec * 1000000 + header.ts.tv_usec;
            uint32_t pkt_size = header.caplen; 
            uint8_t tcp_flag = tcp_header->th_flags;

            // Create a packet object
            Packet packet(src_ip, dst_ip, src_port, dst_port, IPPROTO_TCP, timestamp, ip_header->ip_tos, pkt_size, tcp_flag);

            // Check if the packet match any existing flow
            bool flow_found = false;
            for (auto& flow : active_flows) {
                if (flow.matches(packet)) {
                    flow.addPacket(packet);
                    flow_found = true;
                    break;
                }
            }

            // If no matching flow is found, create a new flow
            if (!flow_found) {
                Flow new_flow;
                flow_sequence++;
                new_flow.addPacket(packet);
                active_flows.push_back(new_flow);
            }

            // Check active/inactive timeouts and add expired flows to buffer
            uint64_t current_time = header.ts.tv_sec * 1000000 + header.ts.tv_usec;
            for (auto it = active_flows.rbegin(); it != active_flows.rend();) {
                if (it->isInactive(current_time, inactive_timeout * 1000000) || it->isActiveExpired(current_time, active_timeout * 1000000)) {
                    // Add expired flow to flow buffer and delete it from active - from latest to newest
                    flow_buffer.push_back(it->toNetFlowRecord());
                    it = decltype(it)(active_flows.erase(std::next(it).base()));

                    // Export flows when buffer has 30 flows
                    if (flow_buffer.size() >= MAX_FLOWS) {
                        // Prepare netflow v5 header data
                        uint32_t timestamp_now = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch()).count();;
                        uint32_t sys_uptime = timestamp_now - timestamp_start;
                        uint32_t unix_secs = header.ts.tv_sec;
                        uint32_t unix_nsecs = header.ts.tv_usec * 1000;

                        //TODO Debug only
                        // std::cout << "header.ts.tv_sec: " << header.ts.tv_sec << std::endl;
                        // std::cout << "header.ts.tv_usec: " << header.ts.tv_usec << std::endl;
                        // std::cout << "current_time: " << current_time << std::endl;
                        // std::cout << "sys_uptime: " << sys_uptime << std::endl;
                        // std::cout << "unix_secs: " << unix_secs << std::endl;
                        // std::cout << "unix_nsecs: " << unix_nsecs << std::endl;

                        // Create a netflow object with header data and add all 30 records from the flow buffer
                        NetflowV5 netflow_v5(sys_uptime, unix_secs, unix_nsecs, flow_sequence);
                        for (const auto& record : flow_buffer) {
                            netflow_v5.addRecord(record);
                        }
                        netflow_v5.prepareHeader();
                        // Export the netflow v5 message
                        netflow_v5.exportToCollector(host.c_str(), std::stoi(port));
                        // Clear the flow bufffer
                        flow_buffer.clear();
                    }
                } else {
                    ++it;
                }
            }
        }
    }

    // End all active flows and push them to the flow_buffer - from latest to newest
    for (auto it = active_flows.rbegin(); it != active_flows.rend();) {
        flow_buffer.push_back(it->toNetFlowRecord());
        it = decltype(it)(active_flows.erase(std::next(it).base()));
    }

    // Export remaining flows from the flow buffer
    if (!flow_buffer.empty()) {
        // Prepare netflow v5 header data
        uint32_t timestamp_now = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch()).count();;
        uint32_t sys_uptime = timestamp_now - timestamp_start;//(current_time / 1000) - timestamp_start; 
        uint32_t unix_secs = header.ts.tv_sec;
        uint32_t unix_nsecs = header.ts.tv_usec * 1000;

        //TODO Debug only
        // std::cout << "header.ts.tv_sec: " << header.ts.tv_sec << std::endl;
        // std::cout << "header.ts.tv_usec: " << header.ts.tv_usec << std::endl;
        // std::cout << "current_time: " << current_time << std::endl;
        // std::cout << "sys_uptime: " << sys_uptime << std::endl;
        // std::cout << "unix_secs: " << unix_secs << std::endl;
        // std::cout << "unix_nsecs: " << unix_nsecs << std::endl;

        // Create a netflow object with header data and add all 30 records from the flow buffer
        NetflowV5 netflow_v5(sys_uptime, unix_secs, unix_nsecs, flow_sequence);
        for (const auto& record : flow_buffer) {
            netflow_v5.addRecord(record);
        }
        netflow_v5.prepareHeader();
        // Export the netflow v5 message
        netflow_v5.exportToCollector(host.c_str(), std::stoi(port));
        // Clear the flow bufffer
        flow_buffer.clear();
    }

    pcap_close(pcap);
    return 0;
}