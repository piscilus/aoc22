#include <stdlib.h>
#include <stdint.h>

typedef struct fifo_node
{
    struct fifo_node* next;
    uint64_t data;
} fifo_node_t;

typedef struct fifo_queue
{
    fifo_node_t *head;
    fifo_node_t *tail;
    size_t n;
} queue_t;

queue_t* queue_init();
void queue_enqueue(queue_t* q, uint64_t data);
int queue_dequeue(queue_t* q, uint64_t* data);
void queue_destroy(queue_t* q);
