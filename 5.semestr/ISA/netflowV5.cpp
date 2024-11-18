/*
ISA project - PCAP NetFlow v5 exporter
Author: Michálek Kryštof (xmicha94) 
*/

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include "netflowV5.h"

// Costructor
NetflowV5::NetflowV5(uint32_t sys_uptime, uint32_t unix_secs, uint32_t unix_nsecs, uint32_t flow_sequence) {
    nf_v5_header.version = htons(5);
    nf_v5_header.count = htons(0);
    nf_v5_header.sys_uptime = htonl(sys_uptime);
    nf_v5_header.unix_secs = htonl(unix_secs);
    nf_v5_header.unix_nsecs = htonl(unix_nsecs);
    nf_v5_header.flow_sequence = htonl(flow_sequence);
    nf_v5_header.engine_type = 0;
    nf_v5_header.engine_id = 0;
    nf_v5_header.sampling_interval = 0;
}
    
// Function to add a flow record - add_record(record);
void NetflowV5::addRecord(const netflowV5Record& record) {
    nf_v5_records.push_back(record);

    //TODO Debugging: Print record details
    // std::cout << "Adding record: " << std::endl
    //           << "src_addr: " << inet_ntoa(*(in_addr*)&record.src_addr) << std::endl
    //           << "dst_addr: " << inet_ntoa(*(in_addr*)&record.dst_addr) << std::endl
    //           << "src_port: " << ntohs(record.src_port) << std::endl
    //           << "dst_port: " << ntohs(record.dst_port) << std::endl
    //           << "dPkts: " << ntohl(record.dPkts) << std::endl
    //           << "dOctets: " << ntohl(record.dOctets) << std::endl
    //           << "first: " << ntohl(record.first) << std::endl
    //           << "last: " << ntohl(record.last) << std::endl
    //           << std::endl;
}

// Function to convert header data from host byte to network byte order
void NetflowV5::prepareHeader() {  
    nf_v5_header.count = htons(nf_v5_records.size()); 
}

// Fuction to export netflow v5 message to the collector
void NetflowV5::exportToCollector(const char* collector_ip, uint16_t collector_port) {
    // Create a socket for UDP
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return;
    }
    struct sockaddr_in collector_addr;
    memset(&collector_addr, 0, sizeof(collector_addr));
    // Set IPv4l, converts port from host byte to network byte order
    collector_addr.sin_family = AF_INET;
    collector_addr.sin_port = htons(collector_port);
    // Converts IPv4 address from text to binary
    inet_pton(AF_INET, collector_ip, &collector_addr.sin_addr);

    // Prepare buffer for the netflow v5 message 
    size_t header_size = sizeof(nf_v5_header);
    size_t record_size = sizeof(netflowV5Record);
    size_t total_size = header_size + (nf_v5_records.size() * record_size);
    char* buffer = new char[total_size];

    //TODO  Debugging: Print header details before copying to buffer
    // std::cout << "version: " << ntohs(nf_v5_header.version) << std::endl;
    // std::cout << "count: " << ntohs(nf_v5_header.count) << std::endl;
    // std::cout << "sys_uptime: " << ntohl(nf_v5_header.sys_uptime) << std::endl;
    // std::cout << "unix_secs: " << ntohl(nf_v5_header.unix_secs) << std::endl;
    // std::cout << "unix_nsecs: " << ntohl(nf_v5_header.unix_nsecs) << std::endl;
    // std::cout << "flow_sequence: " << ntohl(nf_v5_header.flow_sequence) << std::endl;
    // std::cout << "engine_type: " << (int)nf_v5_header.engine_type << std::endl;
    // std::cout << "engine_id: " << (int)nf_v5_header.engine_id << std::endl;
    // std::cout << "sampling_interval: " << ntohs(nf_v5_header.sampling_interval) << std::endl;
    // std::cout << "____________________________" << std::endl;

    // Copy header to buffer
    memcpy(buffer, &nf_v5_header, header_size);

    // Copy records to buffer
    memcpy(buffer + header_size, nf_v5_records.data(), nf_v5_records.size() * record_size);

    // Send the data to the collector
    ssize_t sent_bytes = sendto(sockfd, buffer, total_size, 0, (struct sockaddr*)&collector_addr, sizeof(collector_addr));
    if (sent_bytes < 0) {
        perror("sendto failed");
    } else {
        int bytes = 0;
        for (const auto& record : nf_v5_records) {
            bytes += record.dOctets;
        }
    }

    delete[] buffer;
    close(sockfd);
}

