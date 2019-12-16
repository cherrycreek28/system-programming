#ifndef SERVER_H
#define SERVER_H

#include "queue.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

extern queue_t INBOX[256]; 
extern queue_t OUTBOX[256]; 

int init_server(int server_port);
void* sorter(void* arg);
void* handle_deposit_client(void* arg);
void* handle_retrieve_client(void* arg);

#endif