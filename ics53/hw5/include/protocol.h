#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdlib.h>

// CONNECTION
// client -> type of connection,id
// DATA
// client -> source_id, length of message, msg

enum client_type { DEPOSIT, RETRIEVE };

typedef struct connect_msg {
    uint8_t id;
    enum client_type type;
} connect_msg;

typedef struct {
    uint32_t body_len;
    uint8_t src_id;
    uint8_t dest_id;
    char* msg;
} data_msg;

int send_connect_msg(int socket, connect_msg* connect);
connect_msg* recv_connect_msg(int socket);
void free_connect_msg(connect_msg* msg);

int send_data_msg(int socket, data_msg* data);
data_msg* recv_data_msg(int socket);
void free_data_msg(data_msg* msg);

#endif