/*
 * Simple FIFO queue implementation.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>

/**
 * \brief Object type to handle all queue operations.
 */
typedef struct queue_list queue_t;

/**
 * \brief Initialize queue.
 *
 * This function prepares a queue prior use. The provided item_size
 * must be greater than null.
 *
 * \param[in] item_size Size of item to be stored in the queue instance.
 *
 * \return queue_t* Queue handle or NULL if initialization failed.
 */
queue_t*
queue_init(size_t item_size);

/**
 * \brief Enqueue an item.
 *
 * \param[in,out] q    Handle of the queue.
 * \param[in]     item Item to enqueue.
 *
 * \return int
 * \retval 0 Failure, could not allocate memory.
 * \retval 1 Success.
 */
int
queue_enqueue(queue_t* q, const void* item);

/**
 * \brief Dequeue oldest item.
 *
 * \param[in,out] q    Handle of the queue.
 * \param[out]    item Pointer to store item.
 *
 * \return int
 * \retval 0 Failure, queue empty.
 * \retval 1 Success.
 */
int
queue_dequeue(queue_t* q, void* item);

/**
 * \brief Peek oldest item.
 *
 * \param[in] q    Handle of the queue.
 * \param[out]    item Pointer to store item.
 *
 * \return int
 * \retval 0 Failure, queue empty.
 * \retval 1 Success.
 */
int
queue_peek(const queue_t* q, void* item);

/**
 * \brief Get number of items in queue.
 * \
 * \param[in] q Handle of the queue.
 * \return size_t
 */
size_t
queue_count(const queue_t* q);

/**
 * \brief Destroy the queue and free memory.
 *
 * \param q Handle of the queue.
 */
void
queue_destroy(queue_t* q);

#endif /* QUEUE_H_ */
