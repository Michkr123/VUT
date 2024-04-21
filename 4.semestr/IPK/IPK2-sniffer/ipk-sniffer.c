#include "ipk-sniffer.h"

int argParse(int argc, char *argv[], snifferArguments *snifferArgs)
{
    int option;
    struct option long_options[] = 
    {
        {"interface", optional_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {"port-source", required_argument, 0, 's'},
        {"port-destination", required_argument, 0, 'd'},
        {"tcp", no_argument, 0, 't'},
        {"udp", no_argument, 0, 'u'},
        {"icmp4", no_argument, &snifferArgs->icmp4, 1},
        {"icmp6", no_argument, &snifferArgs->icmp6, 1},
        {"arp", no_argument, &snifferArgs->arp, 1},
        {"ndp", no_argument, &snifferArgs->ndp, 1},
        {"igmp", no_argument, &snifferArgs->igmp, 1},
        {"mld", no_argument, &snifferArgs->mld, 1},
        {0, 0, 0, 0}
    };

    // Parse command-line options
    while ((option = getopt_long(argc, argv, "i::p:tun:", long_options, NULL)) != -1) 
    {
        switch (option) 
        {
            case 'i':
                if(snifferArgs->interface)
                {
                    return 1;
                }
                if(optarg) {
                    snifferArgs->interface = strdup(optarg);
                } 
                else if(argv[optind] && argv[optind][0] != '-') 
                {
                    snifferArgs->interface = strdup(argv[optind]);
                    optind++;
                } 
                else 
                {
                    if(argc == 2)
                    {
                        // No interface provided
                        if(printInterfaces(snifferArgs))
                        {
                            return 1;
                        }  
                    }
                    else
                    {
                        return 1;
                    }
                }
                break;
            case 'p':
                snifferArgs->destination_port = strdup(optarg);
                snifferArgs->source_port = snifferArgs->destination_port;
                if (snifferArgs->destination_port == NULL || snifferArgs->source_port == NULL) 
                {
                    perror("Memory allocation failed");
                    return 1;
                }
                break;
            case 'd':
                snifferArgs->destination_port = strdup(optarg);
                if (snifferArgs->destination_port == NULL) 
                {
                    perror("Memory allocation failed");
                    return 1;
                }
                break;
            case 's':
                snifferArgs->source_port = strdup(optarg);
                if (snifferArgs->source_port == NULL) 
                {
                    perror("Memory allocation failed");
                    return 1;
                }
                break;
            case 't':
                snifferArgs->tcp = 1;
                break;
            case 'u':
                snifferArgs->udp = 1;
                break;
            case 'n':
                snifferArgs->num = atoi(optarg); // Convert argument to integer
                break;
            case 0:
            case 1:
                // This case handles long options with no corresponding short option
                break;
            case '?': // Invalid option
                printf("Usage: %s [-i interface] [-p port | -d port] [-t] [-u] [-a] [-n num]\n", argv[0]);
                return 1;
            default:
                printf("Error: Unknown option\n");
                return 1;
        }
    }
    if(argc == 1)
    {
        if(printInterfaces(snifferArgs))
        {
            return 1;
        }  
    }

    // Additional validation or processing as needed
    /*printf("Interface: %s\n", snifferArgs->interface ? snifferArgs->interface : "(not specified)");
    printf("Port: %s\n", snifferArgs->source_port ? snifferArgs->source_port : "(not specified)");
    printf("Port: %s\n", snifferArgs->destination_port ? snifferArgs->destination_port : "(not specified)");
    printf("TCP: %s\n", snifferArgs->tcp ? "yes" : "no");
    printf("UDP: %s\n", snifferArgs->udp ? "yes" : "no");
    printf("ARP: %s\n", snifferArgs->arp ? "yes" : "no");
    printf("NDP: %s\n", snifferArgs->ndp ? "yes" : "no");
    printf("ICMPv4: %s\n", snifferArgs->icmp4 ? "yes" : "no");
    printf("ICMPv6: %s\n", snifferArgs->icmp6 ? "yes" : "no");
    printf("IGMP: %s\n", snifferArgs->igmp ? "yes" : "no");
    printf("MLD: %s\n", snifferArgs->mld ? "yes" : "no");
    printf("Number: %d\n", snifferArgs->num);*/
    return 0;
}

int printInterfaces(snifferArguments *snifferArgs) 
{
    pcap_if_t *interfaces;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Retrieve the list of available network interfaces
    if (pcap_findalldevs(&interfaces, errbuf) == -1) 
    {
        fprintf(stderr, "Error finding devices: %s\n", errbuf);
        return 1;
    }

    // Print information about each interface
    printf("List of active interfaces:\n");
    pcap_if_t *iface;
    for (iface = interfaces; iface != NULL; iface = iface->next) 
    {
        if (iface->flags & PCAP_IF_UP && iface->flags & PCAP_IF_RUNNING)
        {
            printf("Interface: %s\n", iface->name);
        }
    }

    // Free the list of interfaces
    pcap_freealldevs(interfaces);

    freeAndExit(0, snifferArgs);
    return 0;
}

int main(int argc, char *argv[]) 
{
    snifferArguments *snifferArgs = (snifferArguments*)malloc(sizeof(snifferArguments));

    if (snifferArgs == NULL) 
    {
        perror("Memory allocation failed");
        return 1;
    }

    snifferArgs->interface = NULL;
    snifferArgs->destination_port = NULL;
    snifferArgs->source_port = NULL;
    snifferArgs->tcp = 0;
    snifferArgs->udp = 0;
    snifferArgs->arp = 0;
    snifferArgs->ndp = 0;
    snifferArgs->icmp4 = 0;
    snifferArgs->icmp6 = 0;
    snifferArgs->igmp = 0;
    snifferArgs->mld = 0;
    snifferArgs->num = 1;

    if(argParse(argc, argv, snifferArgs))
    {
        freeAndExit(1, snifferArgs);
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    handle = pcap_open_live(snifferArgs->interface, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) 
    {
        fprintf(stderr, "Couldn't open device: %s\n", errbuf);
        //TODO free function for
        freeAndExit(1, snifferArgs);
    }
    printf("Device opened successfully!\n");

    if (pcap_loop(handle, -1, packet_handler, (u_char *)snifferArgs) != 0) 
    {
        fprintf(stderr, "Error capturing packets: %s\n", pcap_geterr(handle));
        //TODO free function same as above
        freeAndExit(1, snifferArgs);
    }

    pcap_close(handle);
    free(snifferArgs->interface);
    free(snifferArgs->source_port);
    free(snifferArgs);
    return 0;
}

void timestamp(packetInfo *capturedPacket)
{
    char timestamp[32];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", timeinfo);
    
    // Get milliseconds
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    sprintf(timestamp + strlen(timestamp), ".%03ld", spec.tv_nsec / 1000000);

    // Get timezone offset
    long offset = timeinfo->tm_gmtoff / 60;
    sprintf(timestamp + strlen(timestamp), "%+03ld:%02ld", offset / 60, offset % 60);

    strcpy(capturedPacket->timestamp, timestamp);
}

void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {

    snifferArguments *snifferArgs = (snifferArguments *)user;

    //TODO valgrind
    packetInfo *capturedPacket = (packetInfo*)malloc(sizeof(packetInfo));
    if (capturedPacket == NULL) 
    {
        perror("Memory allocation failed");
        freeAndExit(1, snifferArgs);
    }
    
    timestamp(capturedPacket);

    if (filteredPacketCnt == snifferArgs->num) 
    {
        free(capturedPacket);
        freeAndExit(0, snifferArgs);
    }

    // Parse Ethernet header
    struct ether_header *eth_header = (struct ether_header *)packet;
    uint16_t ethertype = ntohs(eth_header->ether_type);

    if (ethertype == ETHERTYPE_IP) 
    {
        // Parse IPv4 header
        struct ip *ip_header = (struct ip *)(packet + sizeof(struct ether_header));
        uint8_t protocol = ip_header->ip_p;

        // Check if the packet is ICMPv4
        if (protocol == IPPROTO_ICMP && snifferArgs->icmp4) 
        {
            struct ether_header *eth_header = (struct ether_header *)packet;
            sprintf(capturedPacket->srcMAC, "%02x:%02x:%02x:%02x:%02x:%02x", eth_header->ether_shost[0], eth_header->ether_shost[1], eth_header->ether_shost[2], eth_header->ether_shost[3], eth_header->ether_shost[4], eth_header->ether_shost[5]);
            sprintf(capturedPacket->dstMAC, "%02x:%02x:%02x:%02x:%02x:%02x", eth_header->ether_dhost[0], eth_header->ether_dhost[1], eth_header->ether_dhost[2], eth_header->ether_dhost[3], eth_header->ether_dhost[4], eth_header->ether_dhost[5]);
            // Get frame length
            capturedPacket->frameLength = pkthdr->len;
            // Get IP addresses
            struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));
            inet_ntop(AF_INET, &ip_header->saddr, capturedPacket->srcIP, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &ip_header->daddr, capturedPacket->dstIP, INET_ADDRSTRLEN);
            // Get ports (not applicable for ICMP)
            capturedPacket->srcPort = 0; // Not applicable for ICMP
            capturedPacket->dstPort = 0; // Not applicable for ICMP
            // Set byte offset to 0 (not applicable for ICMP)
            capturedPacket->byteOffset = 0;
            printPacket(packet, pkthdr, snifferArgs);
            return;
        }

        // Check if the packet is TCP or UDP
        if ((protocol == IPPROTO_TCP && snifferArgs->tcp) || (protocol == IPPROTO_UDP && snifferArgs->udp)) 
        {
            // Check if port filtering is enabled
            if (snifferArgs->destination_port || snifferArgs->source_port)
            {
                // Parse TCP or UDP header
                // Check source and destination ports
                // If the packet matches the port criteria, process and print it
            } 
            else 
            {
                printPacket(packet, pkthdr, snifferArgs);
            }
            return;
        }
    } 
    else if (ethertype == ETHERTYPE_IPV6) 
    {
        // Parse IPv6 header
        struct ip6_hdr *ipv6_header = (struct ip6_hdr *)(packet + sizeof(struct ether_header));
        uint8_t next_header = ipv6_header->ip6_nxt;

        // Check if the packet is ICMPv6
        if (next_header == IPPROTO_ICMPV6 && snifferArgs->icmp6) 
        {
            printPacket(packet, pkthdr, snifferArgs);
            return;
        }

        // Check if the packet is NDP
        if (next_header == IPPROTO_ICMPV6 && snifferArgs->ndp) 
        {
            // Check if the ICMPv6 packet is NDP
            // If so, process and print the NDP packet
            return;
        }

        // Check if the packet is MLD
        if (next_header == IPPROTO_ICMPV6 && snifferArgs->mld) 
        {
            // Check if the ICMPv6 packet is MLD
            // If so, process and print the MLD packet
            return;
        }
    } 
    else if (ethertype == ETHERTYPE_ARP && snifferArgs->arp) 
    {
        printPacket(packet, pkthdr, snifferArgs);
        return;
    } 
    else if (ethertype == ETHERTYPE_IPV6 && snifferArgs->igmp) 
    {
        printPacket(packet, pkthdr, snifferArgs);
        return;
    }
}

void printPacket(const u_char *packet, const struct pcap_pkthdr *pkthdr, snifferArguments *snifferArgs) 
{
    // Print the packet content (you can adjust this based on your requirements)
    //printf("Packet captured\n");

    filteredPacketCnt++;
}

void freeAndExit(int exitVal, snifferArguments *snifferArgs)
{
    if(handle)
    {
        pcap_close(handle);
    }
    free(snifferArgs->interface);
    free(snifferArgs->source_port);
    free(snifferArgs->destination_port);
    free(snifferArgs);
    exit(exitVal);
}