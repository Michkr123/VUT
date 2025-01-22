# ISA project

Michálek Kryštof (xmicha94)

Cílem mojí práce dle zadání bylo implementovat program p2nprobe, který extrahuje informace o síťových
tocích ze souboru PCAP a odešle tyto informace pomocí protokolu UDP na kolektor ve formátu NetFlow v5

## Omezení
Nesedí časové údaje, u větších pcap souborů nesedí počet paketů a bajtů.

## Příklad spuštění
./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]
./p2nprobe  localhost:2055 muj_pcap.pcap -a 5 -i 30
./p2nprobe localhost:2055 muj_pcap.pcap

## Odevzdané soubory
- Makefile
- README.md
- src
	- main.cpp
	- netflow_t.cpp
	- netflow_t.h
	- netflow.cpp
	- netflow.h
	- pcap_reader.cpp
	- pcap_reader.h
	- udp_interface.cpp
    - udp_interface.h