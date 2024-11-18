/*
ISA project - PCAP NetFlow v5 exporter
Author: Michálek Kryštof (xmicha94) 
*/

#include "packet.h"
#include <arpa/inet.h> 

// Constructor
Packet::Packet(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port,
               uint8_t protocol, uint64_t timestamp, uint8_t tos, uint32_t pkt_size, uint8_t tcp_flags)
    : src_ip(src_ip), dst_ip(dst_ip), src_port(src_port), dst_port(dst_port),
      protocol(protocol), timestamp(timestamp), tos(tos), pkt_size(pkt_size), tcp_flags(tcp_flags)  {}

// Getters
uint32_t Packet::getSrcIp() const { return src_ip; }
uint32_t Packet::getDstIp() const { return dst_ip; }
uint16_t Packet::getSrcPort() const { return src_port; }
uint16_t Packet::getDstPort() const { return dst_port; }
uint8_t Packet::getProtocol() const { return protocol; }
uint64_t Packet::getTimestamp() const { return timestamp; }
uint8_t Packet::getTOS() const { return tos; }
uint32_t Packet::getPktSize() const { return pkt_size; }
uint8_t Packet::getTcpFlags() const { return tcp_flags; }