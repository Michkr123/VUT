/*
ISA project - PCAP NetFlow v5 exporter
Author: Michálek Kryštof (xmicha94) 
*/

#include "flow.h"

// Constructor
Flow::Flow() : first_packet_timestamp(0), last_packet_timestamp(0), num_of_packets(0), size_of_flow(0) {}

// Method to add a packet to the flow - addPacket(packet)
void Flow::addPacket(const Packet& packet) {
    packets.push_back(packet);
    // Update timestamps
    uint64_t packetTimestamp = packet.getTimestamp();
    if (first_packet_timestamp == 0) {
        first_packet_timestamp = packetTimestamp;
    }
    last_packet_timestamp = packetTimestamp;
    // Update number of packets and total size
    num_of_packets++;
    size_of_flow += packet.getPktSize();
}

// Getter
uint64_t Flow::getFirstPacketTimestamp() const { return first_packet_timestamp; }
uint64_t Flow::getLastPacketTimestamp() const { return last_packet_timestamp; }
uint32_t Flow::getSizeOfFlow() const { return size_of_flow; }

// Check if the flow is inactive - is_inactive(uint64_t current_timestamp, uint64_t inactive_timeout)
bool Flow::isInactive(uint64_t current_timestamp, uint64_t inactive_timeout) const {
    return (current_timestamp - last_packet_timestamp) > inactive_timeout;
}

// Check if the flow has exceeded the active timeout - is_active_expired(uint64_t current_timestamp, uint64_t active_timeout)
bool Flow::isActiveExpired(uint64_t current_timestamp, uint64_t active_timeout) const {
    return (current_timestamp - first_packet_timestamp) > active_timeout;
}

// Convert flow to NetFlow v5 record - toNetFlowRecord()
netflowV5Record Flow::toNetFlowRecord() const {
    netflowV5Record record;

    // Fill in record fields based on flow information
    record.src_addr = packets[0].getSrcIp();
    record.dst_addr = packets[0].getDstIp();
    record.nexthop = ntohs(0);
    record.input = ntohs(0);
    record.output = htons(0);
    record.dPkts = htonl(num_of_packets);
    record.dOctets = htonl(size_of_flow);
    record.first = htonl((uint32_t)(first_packet_timestamp) / 1000000);
    record.last = htonl((uint32_t)(last_packet_timestamp) / 1000000);
    record.src_port = packets[0].getSrcPort();
    record.dst_port = packets[0].getDstPort();
    record.pad1 = 0;
    record.tcp_flags = packets[0].getTcpFlags();
    record.prot = packets[0].getProtocol();
    record.tos = packets[0].getTOS();
    record.src_as = htons(0);
    record.dst_as = htons(0);
    record.src_mask = 0;
    record.dst_mask = 0;
    record.pad2 = 0;

    return record;
}

// Check if the flow matches a packet (5-tuple match) - matches(const Packet& packet)
bool Flow::matches(const Packet& packet) const {
    return (packet.getSrcIp() == this->packets[0].getSrcIp() &&
            packet.getDstIp() == this->packets[0].getDstIp() &&
            packet.getSrcPort() == this->packets[0].getSrcPort() &&
            packet.getDstPort() == this->packets[0].getDstPort() &&
            packet.getProtocol() == this->packets[0].getProtocol());
}
