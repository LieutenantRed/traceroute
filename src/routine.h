#ifndef __ROUTINES_H__
#define __ROUTINES_H__

#include <stdio.h>

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "components.h"

#define BUFFER_SIZE 100
#define THIS "192.170.2.80"
#define THIS_PORT 1952

#define handle_error(msg) \
	do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

void routepath(char* ip, u_char ttl, int sock);

void getpat(char* ip, int sock, char *buff);

int display(char* text, u_char num, struct _IO_FILE *stream);

#endif