/* Import required header file */
#include "linked_list.h"

// Prevent multiple declarations when included in multiple times
#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED

/* Priority queue structure */
typedef struct
{
    Linked_list *linked_list; // A pointer to a linked list used to implement the priority queue
} Priority_queue;

/**
 * @brief Create a priority queue
 *
 * @return Priority_queue* A pointer to the newly created priority queue
 */
Priority_queue *create_priority_queue();

/* Element structure */
typedef struct
{
    /* This struct will be used to store both the data and priority in the linked list */

    int priority;
    void *data;
} Element;

/**
 * @brief Appends to element on the end of the priority queue
 *
 * @param pq A pointer to the priority queue the element is to be appended to
 * @param elem A pointer to the data we want to append
 * @param priority The priority of element
 */
void p_append(Priority_queue *pq, void *elem, int priority);

/**
 * @brief Serves the next element in priority queue
 *
 * @param pq A pointer to the priority queue that the next element will be served from
 * @return void* A pointer to the data stored in the element
 */
void *p_serve(Priority_queue *pq);

/**
 * @brief Gets the length of the priority queue
 *
 * @param pq A pointer to the priority queue whose length will be returned
 * @return int The length of the priority queue
 */
int p_len(Priority_queue *pq);

/**
 * @brief Sets the priorities of all elements with a specified priority to a new priority
 *
 * @param pq A pointer to the priority queue
 * @param priority_to_change The priority which we want to change
 * @param new_priority The priority that we want to change to
 */
void edit_priorities(Priority_queue *pq, int priority_to_change, int new_priority);

#endif