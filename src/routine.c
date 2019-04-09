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

void getpat(char* ip, int sock, char *buff) {
	uint32_t *rcv_ip_ptr = (char*)buff
					+ sizeof(struct eth_head)
					+ sizeof(struct ip_head)
					- sizeof(uint32_t);
	uint32_t *snd_ip_ptr = (char*)buff
					+ sizeof(struct eth_head)
					+ sizeof(struct ip_head)
					- sizeof(uint32_t)
					- sizeof(uint32_t);
	u_char *proto = (char*)buff
					+ sizeof(struct eth_head)
					+ sizeof(struct ip_head)
					- sizeof(uint32_t) //ip_dst
					- sizeof(uint32_t) //ip_src
					- sizeof(uint16_t) //csum
					- 1; //proto

	uint32_t g_rcv_ip;
	inet_aton(THIS, (struct in_addr *)&(g_rcv_ip));
	uint32_t g_snd_ip;
	inet_aton(ip, (struct in_addr *)&(g_snd_ip));

	while ( (ntohs(*proto) != 1) &&
		(*rcv_ip_ptr != g_rcv_ip) &&
		(*snd_ip_ptr != g_snd_ip)) {
		recvfrom(sock, buff, BUFFER_SIZE, 0, 0, 0);
	}
}

int display(char* text, u_char num, struct _IO_FILE *stream) {
	//11 0 - ttl
	//3 3 - port unreachbl
	struct icmp_head icmp;
	memset(&icmp, 0, sizeof(struct icmp_head));
	memcpy(
		&icmp,
		text + sizeof(struct eth_head) + sizeof(struct ip_head),
		sizeof(struct icmp_head)
	);

	uint32_t *ip_log;
	ip_log = (char *)text
				+ sizeof(struct eth_head)
				+ sizeof(struct ip_head)
				- sizeof(uint32_t)
				- sizeof(uint32_t);

	if (icmp.type == 11)
		fprintf(stream, "%d. %s", (int)num, inet_ntoa(*((struct in_addr *)ip_log)));
	if (icmp.type == 3 && icmp.code == 3)
		return 1;
	else
		return 0;

}