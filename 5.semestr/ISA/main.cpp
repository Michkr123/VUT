#include <iostream>
#include <unistd.h>
#include <pcap.h>
#include <map>
#include <vector>
#include <netinet/ip.h>    // Provides iphdr structure
#include <netinet/tcp.h>   // Provides tcphdr structure
#include <netinet/udp.h>   // Provides udphdr structure
#include <arpa/inet.h>
#include "packet.h"
#include "flow.h"
#include "netflow_v5.h"

const int MAX_FLOWS = 30;

int main(int argc, char* argv[]) {
    std::string host;
    std::string port;
    std::string pcap_file_path;
    int active_timeout = 60;   // Active timeout in seconds
    int inactive_timeout = 60; // Inactive timeout in seconds

    // Parse command-line arguments
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

    // Process remaining arguments (host:port and PCAP file)
    for (int i = optind; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find(':') != std::string::npos) {
            size_t colon_pos = arg.find(':');
            host = arg.substr(0, colon_pos);
            port = arg.substr(colon_pos + 1);
        } else {
            pcap_file_path = arg;
        }
    }

    // Validate required parameters
    if (host.empty() || port.empty() || pcap_file_path.empty()) {
        std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
        return 1;
    }

    // Open the PCAP file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = pcap_open_offline(pcap_file_path.c_str(), errbuf);
    if (pcap == nullptr) {
        std::cerr << "Error opening file: " << errbuf << std::endl;
        return 1;
    }

    // Variables to store packet data
    const u_char* packet_data;
    struct pcap_pkthdr header;

    // List of active flows and buffer to export flows
    std::vector<Flow> active_flows;
    std::vector<netflow_v5_record> flow_buffer;

    // Loop through all packets in the file
    while ((packet_data = pcap_next(pcap, &header)) != nullptr) {
        // Parse packet headers
        struct ip* ip_header = (struct ip*)(packet_data + 14);  // Assuming Ethernet header is 14 bytes

        if (ip_header->ip_p == IPPROTO_TCP) {
            struct tcphdr* tcp_header = (struct tcphdr*)(packet_data + 14 + (ip_header->ip_hl * 4));

            uint32_t src_ip = ntohl(ip_header->ip_src.s_addr);
            uint32_t dst_ip = ntohl(ip_header->ip_dst.s_addr);
            uint16_t src_port = ntohs(tcp_header->source);
            uint16_t dst_port = ntohs(tcp_header->dest);
            uint64_t timestamp = header.ts.tv_sec * 1000000 + header.ts.tv_usec;
            uint32_t pkt_size = header.len; 
            uint8_t tcp_flag = tcp_header->th_flags;  

            // Create a Packet object
            Packet packet(src_ip, dst_ip, src_port, dst_port, IPPROTO_TCP, timestamp, ip_header->ip_tos, pkt_size, tcp_flag);

            // Look for an existing flow that matches the packet (based on 5-tuple)
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
                new_flow.addPacket(packet);
                active_flows.push_back(new_flow);
            }

            // Check active/inactive timeouts and add expired flows to buffer
            uint64_t current_time = header.ts.tv_sec * 1000000 + header.ts.tv_usec;
            for (auto it = active_flows.begin(); it != active_flows.end();) {
                if (it->is_inactive(current_time, inactive_timeout * 1000000) || 
                    it->is_active_expired(current_time, active_timeout * 1000000)) {
                    flow_buffer.push_back(it->toNetFlowRecord());  // Convert to NetFlow record and add to buffer
                    it = active_flows.erase(it); // Remove the flow after adding to buffer
                } else {
                    ++it;
                }
            }

            // Export flows when buffer has 30 flows
            if (flow_buffer.size() >= MAX_FLOWS) {
                // Create a NetFlow v5 message right before exporting
                uint32_t sys_uptime = 0;  // You can replace this with actual uptime if needed
                uint32_t unix_secs = current_time / 1000000;
                uint32_t unix_nsecs = (current_time % 1000000) * 1000;
                uint32_t flow_sequence = 0;  // This should increment with each NetFlow message
                Netflow_v5 netflow_v5(sys_uptime, unix_secs, unix_nsecs, flow_sequence);

                netflow_v5.prepare_header();
                for (const auto& record : flow_buffer) {
                    netflow_v5.add_record(record);  // Add each flow record to the NetFlow message
                }
                netflow_v5.export_to_collector(host.c_str(), std::stoi(port));  // Send to collector
                flow_buffer.clear();  // Clear the buffer after exporting
            }

        }
    }

    // Export remaining flows at the end of the PCAP file
    if (!flow_buffer.empty()) {
        uint64_t current_time = header.ts.tv_sec * 1000000 + header.ts.tv_usec;
        uint32_t sys_uptime = 0;  // You can replace this with actual uptime if needed
        uint32_t unix_secs = current_time / 1000000;
        uint32_t unix_nsecs = (current_time % 1000000) * 1000;
        uint32_t flow_sequence = 0;  // This should increment with each NetFlow message
        Netflow_v5 netflow_v5(sys_uptime, unix_secs, unix_nsecs, flow_sequence);

        netflow_v5.prepare_header();
        for (const auto& record : flow_buffer) {
            netflow_v5.add_record(record);
        }
        netflow_v5.export_to_collector(host.c_str(), std::stoi(port));  // Send remaining flows to the collector
        flow_buffer.clear();
    }

    pcap_close(pcap);
    return 0;
}