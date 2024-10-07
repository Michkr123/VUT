#ifndef FLOW_H
#define FLOW_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "packet.h"

class Flow {
private:
    std::vector<Packet> packets;  // Collection of packets in the flow
    uint64_t firstPacketTimestamp; // Timestamp of the first packet
    uint64_t lastPacketTimestamp;  // Timestamp of the last packet

public:
    // Constructor
    Flow();

    // Method to add a packet to the flow
    void addPacket(const Packet& packet);

    // Getters
    uint64_t getFirstPacketTimestamp() const;
    uint64_t getLastPacketTimestamp() const;

    // Check if the flow is inactive (based on inactive timeout)
    bool isInactive(uint64_t currentTimestamp, uint64_t inactiveTimeout) const;

    // Check if the flow has exceeded the active timeout
    bool isActiveExpired(uint64_t currentTimestamp, uint64_t activeTimeout) const;

    // Export the flow (stub implementation)
    void exportFlow() const;

    // Check if the flow matches a packet (5-tuple match)
    bool matches(const Packet& packet) const; // Removed extra qualification
};

#endif  // FLOW_H
