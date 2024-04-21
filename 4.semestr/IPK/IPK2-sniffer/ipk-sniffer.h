#ifndef sniffer_h
#define sniffer_h

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <pcap.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

int filteredPacketCnt = 0;
pcap_t *handle = NULL;

typedef struct 
{
    char *interface;
    char *source_port;
    char *destination_port;
    int tcp;
    int udp;
    int arp;
    int ndp;
    int icmp4;
    int icmp6;
    int igmp;
    int mld;
    int num;
} snifferArguments;

// Define structure for IP address (supporting both IPv4 and IPv6)
typedef struct 
{
    union 
    {
        uint32_t ipv4;
        uint8_t ipv6[16];
    };
} IPAddress;

// Define structure for MAC address
typedef struct 
{
    uint8_t bytes[6];
} MACAddress;

typedef struct 
{
    char timestamp[32];  // Store timestamp in RFC 3339 format (e.g., "2024-04-12T15:30:00Z")
    MACAddress srcMAC;   // Source MAC address
    MACAddress dstMAC;   // Destination MAC address
    uint16_t frameLength;  // Frame length in bytes
    IPAddress srcIP;     // Source IP address
    IPAddress dstIP;     // Destination IP address
    uint16_t srcPort;    // Source port number (if applicable)
    uint16_t dstPort;    // Destination port number (if applicable)
    uint64_t byteOffset; // Byte offset
    uint8_t byteOffsetHexa[16]; // Hexadecimal representation of bytes
    char byteOffsetASCII[16];   // ASCII representation of bytes
} packetInfo;

int argParse(int argc, char *argv[], snifferArguments *snifferArgs);
int printInterfaces();
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet);
void printPacket(const u_char *packet, const struct pcap_pkthdr *pkthdr, snifferArguments *snifferArgs);
void freeAndExit(int exitVal, snifferArguments *snifferArgs);

#endif