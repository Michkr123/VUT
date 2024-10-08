#include "flow.h"

// Constructor
Flow::Flow() : first_packet_timestamp(0), last_packet_timestamp(0), num_of_packets(0), size_of_flow(0) {}

// Method to add a packet to the flow
void Flow::addPacket(const Packet& packet) {
    packets.push_back(packet);
    
    // Update timestamps
    uint64_t packetTimestamp = packet.getTimestamp();
    if (first_packet_timestamp == 0) {
        first_packet_timestamp = packetTimestamp;
    }
    last_packet_timestamp = packetTimestamp;
    num_of_packets++;
    size_of_flow += packet.getPkt_size();
}

// Getters
uint64_t Flow::getFirst_packet_timestamp() const {
    return first_packet_timestamp;
}

uint64_t Flow::getLast_packet_timestamp() const {
    return last_packet_timestamp;
}

uint8_t Flow::getNum_of_packets() const { return num_of_packets; }

uint32_t Flow::getSize_of_flow() const { return size_of_flow; }

// Check if the flow is inactive (based on inactive timeout)
bool Flow::is_inactive(uint64_t current_timestamp, uint64_t inactive_timeout) const {
    return (current_timestamp - last_packet_timestamp) > inactive_timeout;
}

// Check if the flow has exceeded the active timeout
bool Flow::is_active_expired(uint64_t current_timestamp, uint64_t active_timeout) const {
    return (current_timestamp - first_packet_timestamp) > active_timeout;
}

// Convert flow to NetFlow v5 record
netflow_v5_record Flow::toNetFlowRecord() const {
    netflow_v5_record record;

    // Fill in record fields based on flow information
    record.src_addr = packets[0].getSrc_ip(); // Assuming all packets in the flow have the same source IP
    record.dst_addr = packets[0].getDst_ip(); // Assuming all packets in the flow have the same destination IP
    record.nexthop = 0; // Set as needed
    record.input = 0; // Set as needed (if applicable)
    record.output = 0; // Set as needed (if applicable)
    record.dPkts = num_of_packets; 
    record.dOctets = size_of_flow; 
    record.first = static_cast<uint32_t>(first_packet_timestamp); 
    record.last = static_cast<uint32_t>(last_packet_timestamp); 
    record.src_port = packets[0].getSrc_port(); // Assuming all packets in the flow have the same source port
    record.dst_port = packets[0].getDst_port(); // Assuming all packets in the flow have the same destination port
    record.pad1 = 0;
    record.tcp_flags = packets[0].getTcp_flags(); // Assuming first packet TCP flags
    record.prot = packets[0].getProtocol(); // Protocol type
    record.tos = packets[0].getTOS(); // Type of service
    record.src_as = 0; // Autonomous system numbers (if applicable)
    record.dst_as = 0; // Autonomous system numbers (if applicable)
    record.src_mask = 0; // Source mask (if applicable)
    record.dst_mask = 0; // Destination mask (if applicable)
    record.pad2 = 0;

    return record;
}

// Export the flow (stub implementation)
void Flow::export_flow() const {
    std::cout << "Exporting flow with " << packets.size() << " packets." << std::endl;
    // Additional logic to export flow data can be added here
}

// Check if the flow matches a packet (5-tuple match)
bool Flow::matches(const Packet& packet) const {
    return (packet.getSrc_ip() == this->packets[0].getSrc_ip() &&
            packet.getDst_ip() == this->packets[0].getDst_ip() &&
            packet.getSrc_port() == this->packets[0].getSrc_port() &&
            packet.getDst_port() == this->packets[0].getDst_port() &&
            packet.getProtocol() == this->packets[0].getProtocol());
}
