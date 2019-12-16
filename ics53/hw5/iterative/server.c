#include "debug.h"
#include "protocol.h"
#include "queue.h"
#include "server.h"

#define USEAGE_STR "Server Application Usage: %s -p <port>\n"     \
                   "\t-p\tPort of server\n"

queue_t INBOX[256];
queue_t OUTBOX[256];

void init_queue(queue_t* q) {
	q->head = NULL;
}

int init_server(int server_port) {
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fatalp("socket\n");
    }

    info("Socket successfully created\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(server_port);

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) {
        fatalp("setsockopt");
    }

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        fatalp("bind");
    } 
    info("Socket bind successful\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 1)) != 0) {
        fatalp("listen");
    } 
    success("Server listening on port: %d... Waiting for connection\n", server_port);

    return sockfd;
}

int main(int argc, char *argv[]) {
    int opt;
    unsigned int server_port = 0;

    //Parse the command-line arguments using getopts
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            server_port = atoi(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, USEAGE_STR, argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (server_port == 0) {
        error("Server's port number to listen on is not given\n");
        fprintf(stderr, USEAGE_STR, argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initalize the server listening connection
    int sockfd = init_server(server_port);
    // Initalize the queues
    for (int i = 0; i < 256; i++) {
	    init_queue(&INBOX[i]);
	    init_queue(&OUTBOX[i]);
    }

    struct sockaddr_in client;

    // Listen for connections from clients indefinately
    while(1) {
        socklen_t client_len = sizeof(struct sockaddr_in);

        //Block waiting for a connnection from a client
        int fd = accept(sockfd, (struct sockaddr*)&client, &client_len);
        if (fd < 0) {
            fatalp("accept");
        }
        info("New connection! Socket %d\n", fd);

        // get connection message from client
        connect_msg *cmsg = recv_connect_msg(fd);
        int id = cmsg->id;
        
        // figure out if it's a deposit client or a retrieve client
        if (cmsg->type == DEPOSIT) {
            info("Client %d wants to deposit a message!\n", id);
            data_msg* data;
            while ((data = recv_data_msg(fd)) != NULL) {
                info("Client %d deposited %s for Client %d!\n", id, data->msg, data->dest_id);
                queue_node_t* node = malloc(sizeof(queue_node_t));
                node->data = data;
                node->next = NULL;
                enqueue(&INBOX[id], node);
            }
            close(fd);
            // Move messages from INBOX to OUTBOX for the client
            queue_node_t* nodes;
            nodes = dequeue_all(&INBOX[id]);
            if (nodes != NULL) {
                info("Sorter moving pending messages from Client %d\n", id);
        
                while(nodes != NULL) {
                    data_msg* data = nodes->data;
                    info("Sorter dropping message to Client %d\n", data->dest_id);
                    queue_node_t* rest = nodes->next;
                    nodes->next = NULL;
                    enqueue(&OUTBOX[data->dest_id], nodes);
                    nodes = rest;
                }
            }
        } else {
            info("Client %d wants to retrieve a message!\n", id);
            queue_node_t* nodes = dequeue_all(&OUTBOX[id]); 
            while(nodes != NULL) {
                send_data_msg(fd, nodes->data);
                free_data_msg(nodes->data);
                queue_node_t* ref = nodes;
                nodes = nodes->next;
                free(ref);
            }
            info("Sent all messages for Client %d!\n", id);
            close(fd);
        }
        // free memory 
        free(cmsg);
    }
    return 0;
}
