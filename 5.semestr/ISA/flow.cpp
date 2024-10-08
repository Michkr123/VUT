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

// Check if the flow is inactive (based on inactive timeout)
bool Flow::is_inactive(uint64_t current_timestamp, uint64_t inactive_timeout) const {
    return (current_timestamp - last_packet_timestamp) > inactive_timeout;
}

// Check if the flow has exceeded the active timeout
bool Flow::is_active_expired(uint64_t current_timestamp, uint64_t active_timeout) const {
    return (current_timestamp - first_packet_timestamp) > active_timeout;
}

uint8_t Flow::getNum_of_packets() const { return num_of_packets; }
uint32_t Flow::getSize_of_flow() const { return size_of_flow; }

// Export the flow (stub implementation)
void Flow::export_flow() const {
    std::cout << "Exporting flow with " << packets.size() << " packets." << std::endl;
    // Additional logic to export flow data can be added here
}

bool Flow::matches(const Packet& packet) const {
    return (packet.getSrc_ip() == this->packets[0].getSrc_ip() &&
            packet.getDst_ip() == this->packets[0].getDst_ip() &&
            packet.getSrc_port() == this->packets[0].getSrc_port() &&
            packet.getDst_port() == this->packets[0].getDst_port() &&
            packet.getProtocol() == this->packets[0].getProtocol());
}
