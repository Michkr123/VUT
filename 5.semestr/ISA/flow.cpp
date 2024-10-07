#include "flow.h"

// Constructor
Flow::Flow() : firstPacketTimestamp(0), lastPacketTimestamp(0) {}

// Method to add a packet to the flow
void Flow::addPacket(const Packet& packet) {
    packets.push_back(packet);
    
    // Update timestamps
    uint64_t packetTimestamp = packet.getTimestamp();
    if (firstPacketTimestamp == 0) {
        firstPacketTimestamp = packetTimestamp;
    }
    lastPacketTimestamp = packetTimestamp;
}

// Getters
uint64_t Flow::getFirstPacketTimestamp() const {
    return firstPacketTimestamp;
}

uint64_t Flow::getLastPacketTimestamp() const {
    return lastPacketTimestamp;
}

// Check if the flow is inactive (based on inactive timeout)
bool Flow::isInactive(uint64_t currentTimestamp, uint64_t inactiveTimeout) const {
    return (currentTimestamp - lastPacketTimestamp) > inactiveTimeout;
}

// Check if the flow has exceeded the active timeout
bool Flow::isActiveExpired(uint64_t currentTimestamp, uint64_t activeTimeout) const {
    return (currentTimestamp - firstPacketTimestamp) > activeTimeout;
}

// Export the flow (stub implementation)
void Flow::exportFlow() const {
    std::cout << "Exporting flow with " << packets.size() << " packets." << std::endl;
    // Additional logic to export flow data can be added here
}

bool Flow::matches(const Packet& packet) const {
    return (packet.getSrcIP() == this->packets[0].getSrcIP() &&
            packet.getDstIP() == this->packets[0].getDstIP() &&
            packet.getSrcPort() == this->packets[0].getSrcPort() &&
            packet.getDstPort() == this->packets[0].getDstPort() &&
            packet.getProtocol() == this->packets[0].getProtocol());
}
