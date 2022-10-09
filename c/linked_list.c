/* Import required header file */
#include "../h/linked_list.h"
#include "../h/node.h"
/* Imports required libraries */
#include <stdlib.h>
#include <stdio.h>

/* create_linked_list function */
Linked_list *create_linked_list()
{
    /* Allocate memory for the linked list */
    Linked_list *linked_list = (Linked_list *)malloc(sizeof(Linked_list));

    /* Initialise the linked list */
    linked_list->head = NULL;
    linked_list->length = 0;

    /* Return the linked list */
    return linked_list;
}

/* get_node function */
Node *get_node(Linked_list *l, int index)
{
    // Set the initial node to the head of the linked list
    Node *node = l->head;
    // Loop until we reach the node at the index
    for (int i = 0; i < index; i++)
    {
        // Set the node to the next node every iteration
        node = node->next;
    }
    // Return the node
    return node;
}

/* get_at_index function */
void *get_at_index(Linked_list *l, int index)
{
    /* Check if the index is out of bounds */
    // If the index is greater than the length of the linked list
    if (index > l->length)
    {
        // Print an error message
        perror("Index out of bounds");
        // Exit the program with status code 1
        exit(1);
    }

    /* If the index is not out of bounds */
    // Get the node at the index and return the data
    return get_node(l, index)->data;
}

/* set_at_index function */
void set_at_index(Linked_list *l, int index, void *data)
{
    /* Check if the index is out of bounds */
    // If the index is greater than the length of the linked list
    if (index > l->length)
    {
        // Print an error message
        perror("Index out of bounds");
        // Exit the program with status code 1
        exit(1);
    }

    /* If the index is not out of bounds */
    // Get the node at the index and store it in the variable node
    Node *node = get_node(l, index);
    // Set the data of the node to the data passed
    edit_node(data, node);
}

/* insert_at_index function */
void insert_at_index(Linked_list *l, int index, void *data)
{
    /* Check if the index is out of bounds */
    // If the index is greater than the length of the linked list
    int length = ll_len(l);
    if (index > length)
    {
        // Print an error message
        perror("Index out of bounds");
        // Exit the program with status code 1
        exit(1);
    }

    /* Declare variables */
    Node *prev;     // The previous node
    Node *next;     // The next node
    Node *new_node; // The new node we want to insert

    /* If the index is 0 */
    if (!index)
    {
        // Set the next node to the head of the linked list
        next = l->head;
    }
    else
    {
        // Set the next node to the node at the index - 1
        prev = get_node(l, index - 1);
        // Set the next node to the next node of the previous node
        next = prev->next;
    }

    /* Create a new node */
    new_node = create_node(data, next); // Create a new node with the data passed and the next node

    /* If the index is 0 */
    if (!index)
    {
        // Set the head of the linked list to the new node
        l->head = new_node;
    }
    else
    {
        // Set the next node of the previous node to the new node
        prev->next = new_node;
    }

    /* Increment the length of the linked list */
    l->length++;
}

/* remove_at_index function */
void *remove_at_index(Linked_list *l, int index)
{
    /* Check if the index is out of bounds */
    // If the index is greater than the length of the linked list
    if (index > l->length)
    {
        // Print an error message
        perror("Index out of bounds");
        // Exit the program with status code 1
        exit(1);
    }

    /* Declare variables */
    Node *current; // The current node

    /* Decrease the length of the linked list */
    l->length--;

    /* If the index is 0 */
    if (!index)
    {
        // Set the current node to the head of the linked list
        current = l->head;
        // Set the head of the linked list to the next node of the current node
        l->head = l->head->next;

        // Delete the current node and return the data
        return delete_node(current);
    }
    else
    {
        // Get the node at the index - 1 and store it in the variable prev
        Node *prev = get_node(l, index - 1);
        // Set the current node to the next node of the previous node
        current = prev->next;
        // Set the next node of the previous node to the next node of the current node
        prev->next = current->next;

        // Delete the current node and return the data
        return delete_node(current);
    }
}

/* ll_len function */
int ll_len(Linked_list *l)
{
    // Return the length of the linked list
    return l->length;
}