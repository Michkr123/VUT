#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <netinet/in.h>  // For sockaddr_in structure
#include <iostream>
#include <string>

class Packet {
private:
    uint32_t srcIP;      // Source IP address
    uint32_t dstIP;      // Destination IP address
    uint16_t srcPort;    // Source port number
    uint16_t dstPort;    // Destination port number
    uint8_t protocol;    // Protocol (TCP/UDP/ICMP)
    uint64_t timestamp;  // Timestamp in microseconds
    uint8_t tos;         // Type of Service (TOS)
    
public:
    // Constructor
    Packet(uint32_t srcIP, uint32_t dstIP, uint16_t srcPort, uint16_t dstPort,
           uint8_t protocol, uint64_t timestamp, uint8_t tos);

    // Getters
    uint32_t getSrcIP() const;
    uint32_t getDstIP() const;
    uint16_t getSrcPort() const;
    uint16_t getDstPort() const;
    uint8_t getProtocol() const;
    uint64_t getTimestamp() const;
    uint8_t getTOS() const;

    // Display packet details (for debugging purposes)
    void displayPacketInfo() const;
};

#endif  // PACKET_H
