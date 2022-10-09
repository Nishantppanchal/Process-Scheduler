/* Import required header file */
#include "../h/priority_queue.h"
#include "../h/linked_list.h"
/* Imports required libraries */
#include <stdlib.h>

/* How is the priority queue implemented?
 * The priority queue is implemented as a sorted linked list of elements.
 * A linked list is used to allow easy insertion and deletion of elements as well as variable length.
 * This linked list is sorted in descending order, so the element with the highest priority is at the start of the list.
 * This mean that that a modified version of the binary search algorithm can be used to find the correct position for a new element.
 */

Priority_queue *create_priority_queue()
{
    /* Create a new priority queue */
    Priority_queue *pq = (Priority_queue *)malloc(sizeof(Priority_queue)); // Allocate memory for the priority queue
    pq->linked_list = create_linked_list();                                // Create a linked list for the priority queue

    /* Return the priority queue */
    return pq;
}

/* p_append function */
void p_append(Priority_queue *pq, void *elem, int priority)
{
    /* Creates an new element*/
    Element *element = (Element *)malloc(sizeof(Element)); // Allocates memory for the element
    element->data = elem;                                  // Sets the data of the element to the data passed in
    element->priority = priority;                          // Sets the priority of the element to the priority passed in

    /* Modified binary search algorithm to find the index to insert the element */

    /* Case 1: Length the priority queue is 0 */
    // If the length of priority queue is 0
    if (!p_len(pq))
    {
        // Insert the element into the 0th index of the linked list that is used to implement the priority queue
        insert_at_index(pq->linked_list, 0, element);
        // Exit the function
        return;
    }

    /* Case 2: Length of the priority queue is not 0 */

    /* Declares variables */
    int mid, mid_priority, mid_m_1_priority, mid_p_1_priority;
    Element *mid_element, *mid_m_1_element, *mid_p_1_element;
    /*Declares and initialise variables */
    int low = 0;
    int high = p_len(pq) - 1;

    /* Find insertion point */
    // While the low index is less than or equal to the high index
    while (low <= high)
    {
        /* Get the middle index and priority */
        // Calculate the mid index
        mid = (low + high) / 2;
        // Get the element at the mid index
        mid_element = (Element *)get_at_index(pq->linked_list, mid);
        // Get the priority of the element at the mid index
        mid_priority = mid_element->priority;

        /* Mid is 0 */
        if (!mid)
        {
            // If the priority of the new element is greater than the priority of the element at the mid index
            if (priority > mid_priority)
            {
                // Insert the element at the mid index which is 0
                insert_at_index(pq->linked_list, mid, element);
                // Exit the function
                return;
            }
            // Else if the length of the priority queue is 1
            else if (p_len(pq) == 1)
            {
                // Insert the element at the end of the linked list that is used to implement the priority queue
                insert_at_index(pq->linked_list, mid + 1, element);
                // Exit the function
                return;
            }
        }
        /* Mid is end of linked list used to implement the priority queue */
        else if (mid == p_len(pq) - 1)
        {
            // If the priority of the new element is less than the priority of the element at the mid index
            if (priority < mid_priority)
            {
                // Insert the element at the mid index, which is the end of the linked list
                insert_at_index(pq->linked_list, mid, element);
            }
            else
            {
                // Insert the element at the end of the linked list
                insert_at_index(pq->linked_list, mid + 1, element);
            }
            // Exit the function
            return;
        }

        /* Case 1: Priority of new element is greater than the priority of the element at the mid index */
        if (priority > mid_priority)
        {
            /* Check if the element should be inserted at the mid index */
            // Get the element at the mid index - 1
            mid_m_1_element = (Element *)get_at_index(pq->linked_list, mid - 1);
            // Get the priority of the element at the mid index - 1
            mid_m_1_priority = mid_m_1_element->priority;
            // If the priority of the new element is less than the priority of the element at the mid index - 1
            if (priority < mid_m_1_priority)
            {
                // Insert the element at the mid index
                insert_at_index(pq->linked_list, mid, element);
                // Exit the function
                return;
            }
            // Otherwise, set high to mid - 1 since, the new element can be inserted after mid since the linked list is sorted
            high = mid - 1;
        }
        /* Case 2: Priority of new element is less than or equal to than the priority of the element at the mid index
         * This case is the else case because we want ensure that new elements with the same priority are inserted after the elements
         * with the same priority.
         */
        else
        {
            /* Check if the element should be inserted at the mid + 1 index */
            // Get the element at the mid index + 1
            mid_p_1_element = (Element *)get_at_index(pq->linked_list, mid + 1);
            // Get the priority of the element at the mid index + 1
            mid_p_1_priority = mid_p_1_element->priority;
            // If the priority of the new element is greater than the priority of the element at the mid index + 1
            if (priority > mid_p_1_priority)
            {
                // Insert the element at the mid index + 1
                insert_at_index(pq->linked_list, mid + 1, element);
                // Exit the function
                return;
            }
            // Otherwise, set low to mid + 1 since, the new element can be inserted before mid since the linked list is sorted
            low = mid + 1;
        }
    }
}

/* p_serve function */
void *p_serve(Priority_queue *pq)
{
    /* Case 1: Priority queue is empty */
    // If the priority queue is empty
    if (!p_len(pq))
    {
        // Return NULL
        return NULL;
    }

    /* Case 2: Priority queue is not empty */
    // Get the element at the 0th index of the linked list that is used to implement the priority queue
    Element *element = (Element *)remove_at_index(pq->linked_list, 0);
    // Get the data in the element
    void *data = element->data;
    // Frees the element
    free(element);
    // Return the data of the element
    return data;
}

/* p_len function */
int p_len(Priority_queue *pq)
{
    // Return the length of the linked list that is used to implement the priority queue
    return ll_len(pq->linked_list);
}

/* edit_priorities function */
void edit_priorities(Priority_queue *pq, int priority_to_change, int new_priority)
{
    /* Declares variables */
    Element *element;     // Pointer to an element
    int element_priority; // Priority of an element

    /* Loops through the linked list that is used to implement the priority queue */
    // For each element in the linked list
    for (int i = 0; i < p_len(pq); i++)
    {
        /* Get the ith element's priority */
        // Get the element at the ith index of the linked list
        element = (Element *)get_at_index(pq->linked_list, i);
        // Get the priority of the element
        element_priority = element->priority;

        /* Change the priority of the element if it matches the priority to change */
        // If the priority of the element matches the priority to change
        if (element_priority == priority_to_change)
        {
            // Set the priority of the element to the new priority
            element->priority = new_priority;
        }
    }
}