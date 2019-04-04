#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>

#include "routine.h"

int main(int argc, char** argv) {
	//one socket to send (udp), one socket to recieve (raw)
	int raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (raw == -1) 
		handle_error("RAW socket error");

	int udp = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);               
	if (udp == -1)
		handle_error("UDP socket error");
	int ip_opt_enabled = 1;
	setsockopt(udp,  IPPROTO_IP, IP_HDRINCL, &ip_opt_enabled, sizeof(int));

	//input
	char host_ip[IP_MAX_LENGTH];
	memset(&host_ip, 0, IP_MAX_LENGTH);
	if (argc < 1) 
		handle_error("host ip required");
	else 
		memcpy(host_ip, argv[1], strlen(argv[1]));

	char buffer[BUFFER_SIZE]; //here will be result?
	memset(buffer, 0, BUFFER_SIZE);

	for (u_char ttl = 1; ttl <= 255; ++ttl) {
		routepath(host_ip, ttl, udp);
		getpath(raw, buffer);
		display(buffer, ttl);
		memset(buffer, 0, BUFFER_SIZE);
	}

	fprintf(stdout, "Tracing complete.\n");

	close(raw);
	close(udp);

	return 0;
}