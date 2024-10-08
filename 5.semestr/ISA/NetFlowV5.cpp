#include "NetFlowV5.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>

// Konstruktor pro inicializaci exportéru
NetFlowV5::NetFlowV5(const char* collector_ip, int port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&collector_addr, 0, sizeof(collector_addr));
    collector_addr.sin_family = AF_INET;
    collector_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, collector_ip, &collector_addr.sin_addr) <= 0) {
        perror("Invalid collector IP address");
        exit(EXIT_FAILURE);
    }
}

// Destruktor pro zavření socketu
NetFlowV5::~NetFlowV5() {
    close(sockfd);
}

// Funkce pro odesílání NetFlow v5 toků
void NetFlowV5::sendFlows(const NetFlowV5Header &header, const NetFlowV5Record *records, int flow_count) {
    uint8_t buffer[1500]; // typická velikost UDP paketu
    memset(buffer, 0, sizeof(buffer));

    // Serializace hlavičky a toků do bufferu
    memcpy(buffer, &header, sizeof(NetFlowV5Header));
    memcpy(buffer + sizeof(NetFlowV5Header), records, sizeof(NetFlowV5Record) * flow_count);

    // Odeslání dat na kolektor
    ssize_t sent_size = sendto(sockfd, buffer, sizeof(NetFlowV5Header) + sizeof(NetFlowV5Record) * flow_count, 0,
                               (struct sockaddr*)&collector_addr, sizeof(collector_addr));
    if (sent_size < 0) {
        perror("Sendto failed");
    } else {
        std::cout << "NetFlow datagram sent, size: " << sent_size << " bytes\n";
    }
}
