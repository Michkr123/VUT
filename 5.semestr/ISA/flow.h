/*
ISA project - PCAP NetFlow v5 exporter
Author: Michálek Kryštof (xmicha94) 
*/

#ifndef FLOW_H
#define FLOW_H

#include <vector>
#include <cstdint>
#include <arpa/inet.h>
#include <iostream>
#include "packet.h"
#include "netflowV5.h"

class Flow {
private:
    std::vector<Packet> packets;        // Collection of packets in the flow  
    uint64_t first_packet_timestamp;    // Timestamp of the first packet
    uint64_t last_packet_timestamp;     // Timestamp of the last packet
    uint8_t num_of_packets;             // Number of packets
    uint32_t size_of_flow;              // Size of flow in bytes

public:
    // Constructor
    Flow();

    // Getters
    uint64_t getFirstPacketTimestamp() const;
    uint64_t getLastPacketTimestamp() const;
    uint32_t getSizeOfFlow() const;

    void addPacket(const Packet& packet);
    bool isInactive(uint64_t currentTimestamp, uint64_t inactiveTimeout) const;
    bool isActiveExpired(uint64_t currentTimestamp, uint64_t activeTimeout) const;
    netflowV5Record toNetFlowRecord() const; 
    bool matches(const Packet& packet) const;
};

#endif  // FLOW_H
