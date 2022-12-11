#include "queue.h"

#include <assert.h>

queue_t* queue_init()
{
    queue_t *q = (queue_t*) malloc(sizeof(queue_t));
    assert(q != NULL);

    q->head = NULL;
    q->tail = NULL;
    q->n = 0U;

    return q;
}

void queue_enqueue(queue_t *q, uint64_t data)
{
    fifo_node_t *tmp = malloc(sizeof(fifo_node_t));
    assert(tmp);

    tmp->data = data;
    tmp->next = NULL;

    if (q->n == 0U)
    {
        q->tail = tmp;
        q->head = tmp;
    }
    else
    {
        q->tail->next = tmp;
        q->tail = tmp;
    }

    q->n++;
}

int queue_dequeue(queue_t *q, uint64_t* data)
{
    if (q->n == 0U)
        return 0;

    fifo_node_t *head = q->head;
    *data = head->data;
    q->head = head->next;
    free(head);
    q->n--;

    return 1;
}

void queue_destroy(queue_t *q)
{
    assert(q != NULL);

    fifo_node_t *tmp = q->head;
    fifo_node_t *next = NULL;

    while ((q->n > 0) && (tmp != NULL))
    {
        next = tmp->next;
        free(tmp);
        tmp = next;
        q->n--;
    }

    q->head = NULL;
}
