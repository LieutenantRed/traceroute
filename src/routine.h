#ifndef __ROUTINES_H__
#define __ROUTINES_H__

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "components.h"

#define BUFFER_SIZE 100
#define THIS "192.168.1.3"
#define THIS_PORT 1952

#define handle_error(msg) \
	do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

void routepath(char* ip, u_char ttl, int sock);

void getpath(int sock, char *buff);

void display(char* text, u_char num);

#endif