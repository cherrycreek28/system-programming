#include "debug.h"
#include "protocol.h"
#include "queue.h"
#include "server.h"

#define USEAGE_STR "Server Application Usage: %s -p <port>\n"     \
                   "\t-p\tPort of server\n"

queue_t INBOX[256];
queue_t OUTBOX[256];

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
    success("Server listening on port: %d.. Waiting for connection\n", server_port);

    return sockfd;
}

struct Id_and_Fd
{
    int id; 
    int fd; 
}; 

sem_t ball1; 

void* sorter(void* arg) {
    /*
     * The sorter takes messages from INBOX and puts them in the OUTBOX.
     * Make sure that you use a semaphore to know when work is available!
     */
    while (1)
    {
        sem_wait(&ball1); 
        queue_node_t* nodes;
        int id = 0;
        int counter = 0; 
        for (; id<256; id++)
        {
            pthread_mutex_lock(&INBOX[id].mutex); 
            nodes = dequeue_all(&INBOX[id]);
            pthread_mutex_unlock(&INBOX[id].mutex); 
            if (nodes != NULL) 
            {
                info("Sorter moving pending messages from Client %d\n", id);
                  
                while(nodes != NULL) 
                {
                    data_msg* data = nodes->data;
                    info("Sorter dropping message to Client %d\n", data->dest_id);
                    queue_node_t* rest = nodes->next;
                    nodes->next = NULL;
                    pthread_mutex_lock(&OUTBOX[data->dest_id].mutex); 
                    /*
                    if (OUTBOX[data->dest_id].head == NULL)
                        counter++; 
                    */
                    enqueue(&OUTBOX[data->dest_id], nodes);
                    pthread_mutex_unlock(&OUTBOX[data->dest_id].mutex); 
                    nodes = rest;
                }
            }
        }
        /*
        int i = 0; 
        for (; i<counter; i++)
            sem_post(&ball2); 
        */
        //sem_post(&ball2); 
    }
    return NULL;
}




void* handle_deposit_client(void* arg) {
    /*
     * Handle a deposit client by adding the messages it sends to the appropriate INBOX queue
     */
    //sem_wait(&ball2);  
    int id = ((struct Id_and_Fd* )arg)->id; 
    int fd = ((struct Id_and_Fd* )arg)->fd; 
    free((struct Id_and_Fd* )arg); 

    info("Client %d wants to deposit a message!\n", id);
    data_msg* data;
    while ((data = recv_data_msg(fd)) != NULL) 
    {
        info("Client %d deposited %s for Client %d!\n", id, data->msg, data->dest_id);
        queue_node_t* node = malloc(sizeof(queue_node_t));
        node->data = data;
        node->next = NULL;
        pthread_mutex_lock(&INBOX[id].mutex); 
        enqueue(&INBOX[id], node);
        pthread_mutex_unlock(&INBOX[id].mutex); 
    }
    close(fd);
    sem_post(&ball1);  
    return NULL;
}

void* handle_retrieve_client(void* arg) {
    /*
     * Handle a retrieve client by send all the messages in it's appropriate OUTBOX queue
     */
    int id = ((struct Id_and_Fd* )arg)->id; 
    int fd = ((struct Id_and_Fd* )arg)->fd; 
    free((struct Id_and_Fd* )arg); 

    info("Client %d wants to retrieve a message!\n", id);
    pthread_mutex_lock(&OUTBOX[id].mutex); 
    queue_node_t* nodes = dequeue_all(&OUTBOX[id]); 
    pthread_mutex_unlock(&OUTBOX[id].mutex); 
    while(nodes != NULL) {
        send_data_msg(fd, nodes->data);
        free_data_msg(nodes->data);
        queue_node_t* ref = nodes;
        nodes = nodes->next;
        free(ref);
    }
    info("Sent all messages for Client %d!\n", id);
        close(fd);
    
    return NULL;
}

int main(int argc, char *argv[]) {

    for (int i = 0; i < 256; i++) {
	    init_queue(&INBOX[i]);
	    init_queue(&OUTBOX[i]);
    }

    sem_init(&ball1, 0, 0); 
    
    int opt;
    unsigned int server_port = 0;
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

    int sockfd = init_server(server_port);
    

    //initialize the sorter thread
    pthread_t sorter_id; 
    pthread_create(&sorter_id, NULL, sorter, NULL); 


    struct sockaddr_in client;
    while(1) {
        socklen_t client_len = sizeof(struct sockaddr_in);
        int clientfd = accept(sockfd, (struct sockaddr*)&client, &client_len);
        if (clientfd < 0) {
            fatalp("accept");
        }
        info("New connection! Socket %d\n", clientfd);
        /* 
         * Write code to figure out which type of client 
         * and start appropriate thread here.
         */
        connect_msg *cmsg = recv_connect_msg(clientfd);
        int id = cmsg->id;

        struct Id_and_Fd* current = malloc(sizeof(struct Id_and_Fd)); 
        current->id = id; 
        current->fd = clientfd; 

        if (cmsg->type == DEPOSIT)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, handle_deposit_client, (void* )current); 
        }

        else 
        {
            pthread_t tid;
            pthread_create(&tid, NULL, handle_retrieve_client, (void* )current); 
        }

    }

    return 0;
}