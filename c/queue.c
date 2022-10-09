/* Import required header file */
#include "../h/queue.h"
#include "../h/linked_list.h"
/* Imports required libraries */
#include <stdlib.h>

/* How is the queue implemented?
 * The queue is implemented as a linked list of elements.
 * A linked list is used to allow easy insertion and deletion of elements as well as variable length.
 */

Queue *create_queue()
{
    /* Create a new queue */
    Queue *q = (Queue *)malloc(sizeof(Queue)); // Allocate memory for the queue
    q->linked_list = create_linked_list();     // Create a linked list for the queue

    /* Return the queue */
    return q;
}

/* q_append function */
void q_append(Queue *q, void *elem)
{
    // Insert the element at the end of the linked list that is used to implement the queue
    insert_at_index(q->linked_list, ll_len(q->linked_list), elem);
}

/* q_serve function */
void *q_serve(Queue *q)
{
    // Return the element at the start of the linked list that is used to implement the queue and return it
    return remove_at_index(q->linked_list, 0);
}

/* q_len function */
int q_len(Queue *q)
{
    // Return the length of the linked list that is used to implement the queue
    return ll_len(q->linked_list);
}