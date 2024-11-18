/*
ISA project - PCAP NetFlow v5 exporter
Author: Michálek Kryštof (xmicha94) 
*/

#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <netinet/in.h>
#include <iostream>
#include <string>

class Packet {
private:
    uint32_t src_ip;    // Source IP address
    uint32_t dst_ip;    // Destination IP address
    uint16_t src_port;  // Source port number
    uint16_t dst_port;  // Destination port number
    uint8_t protocol;   // Protocol (TCP/UDP/ICMP)
    uint64_t timestamp; // Timestamp in microseconds
    uint8_t tos;        // Type of Service (TOS)
    uint32_t pkt_size;  // size of packet
    uint8_t tcp_flags;  // flag
    
public:
    // Constructor
    Packet(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port,
           uint8_t protocol, uint64_t timestamp, uint8_t tos, uint32_t pkt_size, uint8_t tcp_flags);

    // Getters
    uint32_t getSrcIp() const;
    uint32_t getDstIp() const;
    uint16_t getSrcPort() const;
    uint16_t getDstPort() const;
    uint8_t getProtocol() const;
    uint64_t getTimestamp() const;
    uint8_t getTOS() const;
    uint32_t getPktSize() const;
    uint8_t getTcpFlags() const;
};

#endif  // PACKET_H
