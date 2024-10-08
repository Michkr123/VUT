#ifndef FLOW_H
#define FLOW_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "packet.h"

class Flow {
private:
    std::vector<Packet> packets;  // Collection of packets in the flow  
    uint64_t first_packet_timestamp; // Timestamp of the first packet
    uint64_t last_packet_timestamp;  // Timestamp of the last packet
    uint8_t num_of_packets;          // Number of packets
    uint32_t size_of_flow;           // Size of flow in bytes

public:
    // Constructor
    Flow();

    // Method to add a packet to the flow
    void addPacket(const Packet& packet);

    // Getters
    uint64_t getFirst_packet_timestamp() const;
    uint64_t getLast_packet_timestamp() const;
    uint8_t getNum_of_packets() const;
    uint32_t getSize_of_flow() const;

    // Check if the flow is inactive (based on inactive timeout)
    bool is_inactive(uint64_t currentTimestamp, uint64_t inactiveTimeout) const;

    // Check if the flow has exceeded the active timeout
    bool is_active_expired(uint64_t currentTimestamp, uint64_t activeTimeout) const;

    // Export the flow (stub implementation)
    void export_flow() const;

    // Check if the flow matches a packet (5-tuple match)
    bool matches(const Packet& packet) const; // Removed extra qualification
};

#endif  // FLOW_H
