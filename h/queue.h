/* Import required header file */
#include "linked_list.h"

// Prevent multiple declarations when included in multiple times
#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

/* Queue structure */
typedef struct
{
    Linked_list *linked_list; // A pointer to a linked list used to implement the queue
} Queue;

/**
 * @brief Create a queue
 * 
 * @return Queue* A pointer to the newly created queue
 */
Queue *create_queue();

/**
 * @brief Appends to element on the end of the queue
 *
 * @param q A pointer to the queue the element is to be appended to
 * @param elem A pointer to the data we want to append
 */
void q_append(Queue *q, void *elem);

/**
 * @brief Serves the next element in queue
 *
 * @param q A pointer to the queue that the next element will be served from
 * @return void* A pointer to the data stored in the element
 */
void *q_serve(Queue *q);

/**
 * @brief Gets the length of the queue
 *
 * @param q A pointer to the queue whose length will be returned
 * @return int The length of the queue
 */
int q_len(Queue *q);

#endif