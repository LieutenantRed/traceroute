#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#define IP_MAX_LENGTH 15

#define handle_error(msg) \
    do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

struct ip_head {
	unsigned char ihl:4, ver:4; 
	unsigned char tos;
	uint16_t len;
	uint16_t id;
	uint16_t flg_off;
	unsigned char ttl;
	unsigned char proto;
	uint16_t csum;
	uint32_t ip_src;
	uint32_t ip_dst;
};

struct udp_head {
	uint16_t src;
	uint16_t dst;
	uint16_t len;
	uint16_t csum;
};

struct eth_head {
	u_char	ether_dhost[6];
	u_char	ether_shost[6];
	u_short	ether_type;
};

struct icmp_head {
	u_char type;
	u_char code;
	//only this fields matter
};

uint16_t ip_checksum(void* ip_p);

uint32_t crc32(unsigned char *buf, size_t len);

#endif