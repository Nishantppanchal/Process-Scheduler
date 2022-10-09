/* Import required header file */
#include "../h/node.h"
/* Imports required libraries */
#include <stdlib.h>

/* create_node function */
Node *create_node(void *data, Node *next)
{
    /* Create node */
    Node *node = (Node *)malloc(sizeof(Node)); // Allocate memory for the node

    /* Set node data */
    node->data = data; // Set the data of the node to the data passed
    node->next = next; // Set the next node of the node to the next node passed

    /* Return node */
    return node;
}

/* edit_node function */
void edit_node(void *new_data, Node *node)
{
    /* Set node data */
    node->data = new_data; // Set the data of the node to the data passed
}

/* delete_node function */
void *delete_node(Node *node)
{
    // Get the data of the node
    void *data = node->data;
    // Free the memory allocated for the node
    free(node);
    // Return the data of the node
    return data;
}