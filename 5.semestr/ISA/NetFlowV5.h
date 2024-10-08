#ifndef NETFLOW_V5_H
#define NETFLOW_V5_H

#include <cstdint>
#include <netinet/in.h>

// NetFlow v5 format
// viz https://www.cisco.com/c/en/us/td/docs/net_mgmt/netflow_collection_engine/3-6/user/guide/format.html 

// Struktura NetFlow v5 hlavičky
struct NetFlowV5Header {
    uint16_t version;
    uint16_t count;
    uint32_t sys_uptime;
    uint32_t unix_secs;
    uint32_t unix_nsecs;
    uint32_t flow_sequence;
    uint8_t  engine_type;
    uint8_t  engine_id;
    uint16_t sampling_interval;
};

// Struktura NetFlow v5 záznamu o toku
struct NetFlowV5Record {
    uint32_t src_addr;
    uint32_t dst_addr;
    uint32_t nexthop;
    uint16_t input;
    uint16_t output;
    uint32_t dPkts;
    uint32_t dOctets;
    uint32_t first;
    uint32_t last;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t  pad1;
    uint8_t  tcp_flags;
    uint8_t  prot;
    uint8_t  tos;
    uint16_t src_as;
    uint16_t dst_as;
    uint8_t  src_mask;
    uint8_t  dst_mask;
    uint16_t pad2;
};

class NetFlowV5 {
public:
    NetFlowV5(const char* collector_ip, int port);
    ~NetFlowV5();

    void sendFlows(const NetFlowV5Header &header, const NetFlowV5Record *records, int flow_count);

private:
    int sockfd;
    struct sockaddr_in collector_addr;
};

#endif // NETFLOW_EXPORTER_H
