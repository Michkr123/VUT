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

    // Output the parameters
    std::cout << "Host: " << host << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "PCAP File Path: " << pcap_file_path << std::endl;
    std::cout << "Active Timeout: " << active_timeout << " seconds" << std::endl;
    std::cout << "Inactive Timeout: " << inactive_timeout << " seconds" << std::endl;

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

    // Map of flows (5-tuple key -> Flow object)
    std::vector<Flow> flows;

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

            // Create a Packet object
            Packet packet(src_ip, dst_ip, src_port, dst_port, IPPROTO_TCP, timestamp, ip_header->ip_tos);

            // Look for an existing flow that matches the packet (based on 5-tuple)
            bool flow_found = false;
            for (auto& flow : flows) {
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
                flows.push_back(new_flow);
            }

            // Check active/inactive timeouts and export flows if needed
            uint64_t current_time = header.ts.tv_sec * 1000000 + header.ts.tv_usec;
            for (auto it = flows.begin(); it != flows.end();) {
                if (it->isInactive(current_time, inactive_timeout * 1000000) || 
                    it->isActiveExpired(current_time, active_timeout * 1000000)) {
                    it->exportFlow(); // Export the flow
                    it = flows.erase(it); // Remove the flow after exporting
                } else {
                    ++it;
                }
            }
        }
    }

    pcap_close(pcap);
    return 0;
}


// #include <iostream>
// #include <unistd.h>
// #include <pcap.h>

// int main(int argc, char* argv[]) {
//     std::string host;
//     std::string port;
//     std::string pcap_file_path;
//     int active_timeout = 60;
//     int inactive_timeout = 60;

//     int opt;
//     while ((opt = getopt(argc, argv, "a:i:")) != -1) {
//         switch (opt) {
//             case 'a':
//                 active_timeout = std::stoi(optarg); //TODO funkce na try catch atoi
//                 break;
//             case 'i':
//                 inactive_timeout = std::stoi(optarg); //TODO funkce na try catch atoi
//                 break;
//             default: 
//                 std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
//                 return 1;
//         }
//     }

//     // Second pass: process remaining arguments (host:port and PCAP file)
//     for (int i = optind; i < argc; ++i) {
//         std::string arg = argv[i];
//         if (arg.find(':') != std::string::npos) {
//             // Argument is in the form host:port
//             size_t colon_pos = arg.find(':');
//             host = arg.substr(0, colon_pos);
//             port = arg.substr(colon_pos + 1);
//         } else {
//             // Argument is assumed to be the PCAP file path
//             pcap_file_path = arg;
//         }
//     }

//     // Validate that the required parameters were provided
//     if (host.empty() || port.empty() || pcap_file_path.empty()) {
//         std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
//         return 1;
//     }

//      // Output the parameters
//     std::cout << "Host: " << host << std::endl;
//     std::cout << "Port: " << port << std::endl;
//     std::cout << "PCAP File Path: " << pcap_file_path << std::endl;
//     std::cout << "Active Timeout: " << active_timeout << " seconds" << std::endl;
//     std::cout << "Inactive Timeout: " << inactive_timeout << " seconds" << std::endl;


//     char errbuf[PCAP_ERRBUF_SIZE];   // Buffer for error messages

//     pcap_t* pcap = pcap_open_offline(pcap_file_path.c_str(), errbuf);
//     if (pcap == nullptr) {
//         std::cerr << "Error opening file: " << errbuf << std::endl;
//         return 1;
//     }

//     // Variables to store packet data
//     const u_char* packet;
//     struct pcap_pkthdr header; // Contains packet header (timestamp and length)

//     // Loop through all the packets in the file
//     while ((packet = pcap_next(pcap, &header)) != nullptr) {
        
//     }

//     pcap_close(pcap);
//     return 0;
// }

