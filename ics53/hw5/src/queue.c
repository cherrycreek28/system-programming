#include "queue.h"

void init_queue(queue_t* q) {
    assert(q != NULL);
    /* Initialize queue head and mutex here */

    q->head = NULL; 
    pthread_mutex_init(&(q->mutex), NULL); 
}
