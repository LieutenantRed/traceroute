#include "routine.h"

void routepath(char* ip, u_char ttl, int sock) {
	struct ip_head iphdr;
	memset(&iphdr, 0, sizeof(struct ip_head));
	struct udp_head udphdr;
	memset(&udphdr, 0, sizeof(struct udp_head));
	char buff[BUFFER_SIZE];
	memset(buff, 0, BUFFER_SIZE);

	iphdr.ihl = 5;
	iphdr.ver = 4;
	iphdr.ttl = ttl;
	iphdr.proto = 17;
	inet_aton(THIS, (struct in_addr *)&(iphdr.ip_src));
	inet_aton(ip, (struct in_addr *)&(iphdr.ip_dst));

	udphdr.src = htons(THIS_PORT);
	udphdr.dst = htons(33439);
	udphdr.len = htons(BUFFER_SIZE - sizeof(struct ip_head));
	udphdr.csum = 0;

	memcpy(buff, &iphdr, sizeof(struct ip_head));
	memcpy(buff + sizeof(struct ip_head), &udphdr, sizeof(struct udp_head));

	struct sockaddr_in hostaddr;
	memset(&hostaddr, 0, sizeof(struct sockaddr_in));

	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(33439);
	inet_aton(ip, (struct in_addr *)&(hostaddr.sin_addr));

	sendto (
		sock, 
		buff, 
		BUFFER_SIZE, 
		0, 
		(struct sockaddr *)&hostaddr, 
		sizeof(struct sockaddr_in)
	);
}

void getpath(int sock, char *buff) {
	uint32_t *ip_ptr = (char*)buff 
					+ sizeof(struct eth_head)  
					+ sizeof(struct ip_head) 
					- sizeof(uint32_t);
	u_char *proto = (char*)buff
					+ sizeof(struct eth_head)
					+ sizeof(struct ip_head)
					- sizeof(uint32_t) //ip_dst
					- sizeof(uint32_t) //ip_src
					- sizeof(uint16_t) //csum
					- 1; //proto

	uint32_t current_ip;
	inet_aton(THIS, (struct in_addr *)&(current_ip));

	while ((ntohs(*proto) != 1) && (*ip_ptr != current_ip)) 
		recvfrom(sock, buff, BUFFER_SIZE, 0, 0, 0);
}

void display(char* text, u_char num) {
	//11 0 - ttl 
	//3 3 - port unreachbl
	struct icmp_head icmp;
	memset(&icmp, 0, sizeof(struct icmp_head));

	memcpy(
		&icmp, 
		text + sizeof(struct eth_head) + sizeof(struct ip_head), 
		sizeof(struct icmp_head)
	);

	if (icmp.type == 11)
		//TODO::OUTPUT


}