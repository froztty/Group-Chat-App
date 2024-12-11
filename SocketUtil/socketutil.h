#ifndef SOCKETUTIL_SOCKETUTIL_H
#define SOCKETUTIL_SOCKETUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h> // for close()
#include <stdbool.h>
#include <pthread.h>

struct sockaddr_in* createIPv4Address(char *ip, int port);

int createTCPIPv4Socket();

#endif