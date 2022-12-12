/*
 * Simple FIFO queue implementation.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "queue.h"

#include <assert.h>
#include <string.h>

typedef struct node
{
    void* item;
    struct node *next;
} node_t;

struct queue_list
{
    size_t count;
    size_t item_size;
    node_t *head;
    node_t *tail;
};

queue_t*
queue_init(size_t item_size)
{
    assert(item_size > 0U);

    queue_t* q = (queue_t*) malloc(sizeof(queue_t));
    if (q == NULL)
    {
        return NULL;
    }

    q->count = 0U;
    q->item_size = item_size;
    q->head = q->tail = NULL;

    return q;
}

int
queue_enqueue(queue_t* q, const void* item)
{
    assert(q != NULL);
    assert(item != NULL);

    node_t* new = (node_t*) malloc(sizeof(node_t));
    if (new == NULL)
    {
        return 0;
    }

    new->item = malloc(q->item_size);
    if (new->item == NULL)
    {
        free(new);
        return 0;
    }

    new->next = NULL;

    (void) memcpy(new->item, item, q->item_size);

    if (q->count == 0U)
    {
        q->head = q->tail = new;
    }
    else
    {
        q->tail->next = new;
        q->tail = new;
    }

    q->count++;

    return 1;
}

int
queue_dequeue(queue_t* q, void* item)
{
    assert(q != NULL);
    assert(item != NULL);

    if (q->count == 0U)
    {
        return 0;
    }

    node_t *tmp = q->head;
    (void) memcpy(item, tmp->item, q->item_size);

    if (q->count > 1U)
    {
        q->head = q->head->next;
    }
    else
    {
        q->head = NULL;
        q->tail = NULL;
    }

    q->count--;
    free(tmp->item);
    free(tmp);

    return 1;
}

int
queue_peek(const queue_t* q, void* item)
{
    assert(q != NULL);
    assert(item != NULL);

    if (q->count == 0U)
    {
        return 0;
    }
    else
    {
       node_t* tmp = q->head;
       (void) memcpy(item, tmp->item, q->item_size);
       return 1;
    }
}

size_t
queue_count(const queue_t* q)
{
    return q->count;
}

void
queue_destroy(queue_t* q)
{
    node_t* tmp;

    while (q->count > 0U)
    {
        tmp = q->head;
        q->head = tmp->next;
        free(tmp->item);
        free(tmp);
        q->count--;
    }

    q->item_size = 0U;
    q->head = q->tail = NULL;
}
