#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <assert.h>

typedef struct queue_node_t {
    void* data;
    struct queue_node_t* next;
} queue_node_t;

typedef struct {
    pthread_mutex_t mutex;
    queue_node_t* head;
} queue_t;

void init_queue(queue_t* q);
void enqueue(queue_t* q, queue_node_t* in);
queue_node_t* dequeue_all(queue_t* q);

#endif