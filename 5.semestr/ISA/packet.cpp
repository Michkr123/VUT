#include "packet.h"
#include <arpa/inet.h>  // For inet_ntoa, inet_addr

// Constructor implementation
Packet::Packet(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port,
               uint8_t protocol, uint64_t timestamp, uint8_t tos, uint32_t pkt_size, uint8_t tcp_flag)
    : src_ip(src_ip), dst_ip(dst_ip), src_port(src_port), dst_port(dst_port),
      protocol(protocol), timestamp(timestamp), tos(tos), pkt_size(pkt_size), tcp_flag(tcp_flag)  {}

// Getters implementation
uint32_t Packet::getSrc_ip() const { return src_ip; }
uint32_t Packet::getDst_ip() const { return dst_ip; }
uint16_t Packet::getSrc_port() const { return src_port; }
uint16_t Packet::getDst_port() const { return dst_port; }
uint8_t Packet::getProtocol() const { return protocol; }
uint64_t Packet::getTimestamp() const { return timestamp; }
uint8_t Packet::getTOS() const { return tos; }
uint32_t Packet::getPkt_size() const { return pkt_size; }
uint8_t Packet::getTcp_flag() const { return tcp_flag; }

// Display packet info for debugging
// void Packet::displayPacketInfo() const {
//     std::cout << "Source IP: " << inet_ntoa({src_ip})
//               << ", Source Port: " << src_port
//               << ", Destination IP: " << inet_ntoa({dst_ip})
//               << ", Destination Port: " << dst_port
//               << ", Protocol: " << (int)protocol
//               << ", Timestamp: " << timestamp
//               << ", TOS: " << (int)tos
//               << ", Packet size: " << (int)pkt_size
//               <<  ", Tcp flag: " << tcp_flag
//               << std::endl;
// }
