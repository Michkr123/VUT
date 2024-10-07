#include <iostream>
#include <unistd.h>
#include <pcap.h>

int main(int argc, char* argv[]) {
    std::string host;
    std::string port;
    std::string pcap_file_path;
    int active_timeout = 60;
    int inactive_timeout = 60;

    int opt;
    while ((opt = getopt(argc, argv, "a:i:")) != -1) {
        switch (opt) {
            case 'a':
                active_timeout = std::stoi(optarg); //TODO funkce na try catch atoi
                break;
            case 'i':
                inactive_timeout = std::stoi(optarg); //TODO funkce na try catch atoi
                break;
            default: 
                std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
                return 1;
        }
    }

    // Second pass: process remaining arguments (host:port and PCAP file)
    for (int i = optind; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find(':') != std::string::npos) {
            // Argument is in the form host:port
            size_t colon_pos = arg.find(':');
            host = arg.substr(0, colon_pos);
            port = arg.substr(colon_pos + 1);
        } else {
            // Argument is assumed to be the PCAP file path
            pcap_file_path = arg;
        }
    }

    // Validate that the required parameters were provided
    if (host.empty() || port.empty() || pcap_file_path.empty()) {
        std::cerr << "Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]" << std::endl;
        return 1;
    }

     // Output the parameters
    std::cout << "Host: " << host << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "PCAP File Path: " << pcap_file_path << std::endl;
    std::cout << "Active Timeout: " << active_timeout << " seconds" << std::endl;
    std::cout << "Inactive Timeout: " << inactive_timeout << " seconds" << std::endl;


    char errbuf[PCAP_ERRBUF_SIZE];   // Buffer for error messages

    pcap_t* pcap = pcap_open_offline(pcap_file_path.c_str(), errbuf);
    if (pcap == nullptr) {
        std::cerr << "Error opening file: " << errbuf << std::endl;
        return 1;
    }

    // Variables to store packet data
    const u_char* packet;
    struct pcap_pkthdr header; // Contains packet header (timestamp and length)

    // Loop through all the packets in the file
    while ((packet = pcap_next(pcap, &header)) != nullptr) {
        
    }

    pcap_close(pcap);
    return 0;
}
