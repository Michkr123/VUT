#include "packet.h"
#include <arpa/inet.h>  // For inet_ntoa, inet_addr

// Constructor implementation
Packet::Packet(uint32_t srcIP, uint32_t dstIP, uint16_t srcPort, uint16_t dstPort,
               uint8_t protocol, uint64_t timestamp, uint8_t tos)
    : srcIP(srcIP), dstIP(dstIP), srcPort(srcPort), dstPort(dstPort),
      protocol(protocol), timestamp(timestamp), tos(tos) {}

// Getters implementation
uint32_t Packet::getSrcIP() const { return srcIP; }
uint32_t Packet::getDstIP() const { return dstIP; }
uint16_t Packet::getSrcPort() const { return srcPort; }
uint16_t Packet::getDstPort() const { return dstPort; }
uint8_t Packet::getProtocol() const { return protocol; }
uint64_t Packet::getTimestamp() const { return timestamp; }
uint8_t Packet::getTOS() const { return tos; }

// Display packet info for debugging
void Packet::displayPacketInfo() const {
    std::cout << "Source IP: " << inet_ntoa({srcIP})
              << ", Source Port: " << srcPort
              << ", Destination IP: " << inet_ntoa({dstIP})
              << ", Destination Port: " << dstPort
              << ", Protocol: " << (int)protocol
              << ", Timestamp: " << timestamp
              << ", TOS: " << (int)tos << std::endl;
}
