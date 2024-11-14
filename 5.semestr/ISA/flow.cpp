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
    record.src_addr = packets[0].getSrc_ip(); // Convert to network byte order
    record.dst_addr = packets[0].getDst_ip(); // Convert to network byte order
    record.nexthop = ntohs(0); // Set as needed, converted to network byte order
    record.input = ntohs(0); // Set as needed (if applicable), converted to network byte order
    record.output = htons(0); // Set as needed (if applicable), converted to network byte order
    record.dPkts = htonl(num_of_packets);
    record.dOctets = htonl(size_of_flow);
    record.first = htonl(static_cast<uint32_t>(first_packet_timestamp / 1000));
    record.last = htonl(static_cast<uint32_t>(last_packet_timestamp / 1000));
    record.src_port = packets[0].getSrc_port(); // Convert to network byte order
    record.dst_port = packets[0].getDst_port(); // Convert to network byte order
    record.pad1 = 0;
    record.tcp_flags = packets[0].getTcp_flags(); // TCP flags are single byte, no conversion needed
    record.prot = packets[0].getProtocol(); // Protocol type, no conversion needed
    record.tos = packets[0].getTOS(); // Type of service, no conversion needed
    record.src_as = htons(0); // Autonomous system numbers (if applicable), converted to network byte order
    record.dst_as = htons(0); // Autonomous system numbers (if applicable), converted to network byte order
    record.src_mask = 0; // Source mask (if applicable)
    record.dst_mask = 0; // Destination mask (if applicable)
    record.pad2 = 0;

    // Debugging: Print record details
    // std::cout << "src_addr: " << inet_ntoa(*(in_addr*)&record.src_addr) << std::endl;
    // std::cout << "dst_addr: " << inet_ntoa(*(in_addr*)&record.dst_addr) << std::endl;
    // std::cout << "nexthop: " << inet_ntoa(*(in_addr*)&record.nexthop) << std::endl;
    // std::cout << "input: " << ntohs(record.input) << std::endl;
    // std::cout << "output: " << ntohs(record.output) << std::endl;
    // std::cout << "dPkts: " << ntohl(record.dPkts) << std::endl;
    // std::cout << "dOctets: " << ntohl(record.dOctets) << std::endl;
    // std::cout << "first: " << ntohl(record.first) << std::endl;
    // std::cout << "last: " << ntohl(record.last) << std::endl;
    // std::cout << "src_port: " << ntohs(record.src_port) << std::endl;
    // std::cout << "dst_port: " << ntohs(record.dst_port) << std::endl;
    // std::cout << "pad1: " << (int)record.pad1 << std::endl;
    // std::cout << "tcp_flags: " << (int)record.tcp_flags << std::endl;
    // std::cout << "prot: " << (int)record.prot << std::endl;
    // std::cout << "tos: " << (int)record.tos << std::endl;
    // std::cout << "src_as: " << ntohs(record.src_as) << std::endl;
    // std::cout << "dst_as: " << ntohs(record.dst_as) << std::endl;
    // std::cout << "src_mask: " << (int)record.src_mask << std::endl;
    // std::cout << "dst_mask: " << (int)record.dst_mask << std::endl;
    // std::cout << "pad2: " << ntohs(record.pad2) << std::endl;
    // std::cout << "_________________________________________" << std::endl;

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
