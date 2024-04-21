CC=gcc
CFLAGS= -std=gnu17 -pedantic -Wall -Wextra -g

main: ipk-sniffer.c
	clear
	$(CC) $(CFLAGS) ipk-sniffer.c -o ipk-sniffer -lpcap

clean:
	rm -f ipk-sniffer