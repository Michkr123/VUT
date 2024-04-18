#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>

int argParse(int argc, char *argv[], char **interface, char **destination_port, char **source_port, int *tcp, int *udp, int *arp, int *ndp, int *icmp4, int *icmp6, int *igmp, int *mld, int *num)
{
    int option;
    struct option long_options[] = {
        {"interface", optional_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {"port-source", required_argument, 0, 's'},
        {"port-destination", required_argument, 0, 'd'},
        {"tcp", no_argument, 0, 't'},
        {"udp", no_argument, 0, 'u'},
        {"icmp4", no_argument, icmp4, 1},
        {"icmp6", no_argument, icmp6, 2},
        {"arp", no_argument, arp, 3},
        {"ndp", no_argument, ndp, 4},
        {"igmp", no_argument, igmp, 5},
        {"mld", no_argument, mld, 6},
        {0, 0, 0, 0}
    };

    // Parse command-line options
    while ((option = getopt_long(argc, argv, "i::p:tun:", long_options, NULL)) != -1) {
        switch (option) {
            case 'i':
                if (optarg) {
                    *interface = strdup(optarg);
                } else if (argv[optind] && argv[optind][0] != '-') {
                    *interface = strdup(argv[optind]);
                    optind++;
                } else {
                    // No interface provided
                    //TODO vse ostatni false => vypise aktivni interfaces
                }
                break;
            case 'p':
                *destination_port = strdup(optarg);
                *source_port = *destination_port;
                if (*destination_port == NULL || *source_port == NULL) 
                {
                    perror("Memory allocation failed");
                    return 1;
                }
                break;
            case 'd':
                *destination_port = strdup(optarg);
                if (*destination_port == NULL) 
                {
                    perror("Memory allocation failed");
                    return 1;
                }
                break;
            case 's':
                *source_port = strdup(optarg);
                if (*source_port == NULL) 
                {
                    perror("Memory allocation failed");
                    return 1;
                }
                break;
            case 't':
                *tcp = 1;
                break;
            case 'u':
                *udp = 1;
                break;
            case 'n':
                *num = atoi(optarg); // Convert argument to integer
                break;
            case 0:
                // This case handles long options with no corresponding short option
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case '?': // Invalid option
                printf("Usage: %s [-i interface] [-p port | -d port] [-t] [-u] [-a] [-n num]\n", argv[0]);
                return 1;
            default:
                printf("Error: Unknown option\n");
                return 1;
        }
    }

    // Additional validation or processing as needed
    printf("Interface: %s\n", *interface ? *interface : "(not specified)");
    printf("Port: %s\n", *source_port ? *source_port : "(not specified)");
    printf("Port: %s\n", *destination_port ? *destination_port : "(not specified)");
    printf("TCP: %s\n", *tcp ? "yes" : "no");
    printf("UDP: %s\n", *udp ? "yes" : "no");
    printf("ARP: %s\n", *arp ? "yes" : "no");
    printf("NDP: %s\n", *ndp ? "yes" : "no");
    printf("ICMPv4: %s\n", *icmp4 ? "yes" : "no");
    printf("ICMPv6: %s\n", *icmp6 ? "yes" : "no");
    printf("IGMP: %s\n", *igmp ? "yes" : "no");
    printf("MLD: %s\n", *mld ? "yes" : "no");
    printf("Number: %d\n", *num);
    return 0;
}

int main(int argc, char *argv[]) {
    char *interface = NULL;
    char *destination_port = NULL;
    char *source_port = NULL;
    int tcp = 0;
    int udp = 0;
    int arp = 0;
    int ndp = 0;
    int icmp4 = 0;
    int icmp6 = 0;
    int igmp = 0;
    int mld = 0;
    int num = 0;

    if(argParse(argc, argv, &interface, &destination_port, &source_port, &tcp, &udp, &arp, &ndp, &icmp4, &icmp6, &igmp, &mld, &num))
    {
        return 1;
    }
    
    free(interface);
    free(source_port);
    return 0;
}