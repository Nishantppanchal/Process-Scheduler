/* Import required header file */
#include "node.h"

// Prevent multiple declarations when included in multiple times
#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

typedef struct
{
    Node *head; // The first node (head) in the the linked list
    int length; // The length of the linked list
} Linked_list;

/**
 * @brief Create a linked list
 * 
 * @return Linked_list* A pointer to the newly created linked list
 */
Linked_list *create_linked_list();

/**
 * @brief Gets a node at an index in a linked list
 * @attention This function is not meant to be used outside of this file and hence is not declared in the header file
 *
 * @param l The linked list
 * @param index The index of the node we want to get
 * @return Node* A pointer to the node
 */
Node *get_node(Linked_list *l, int index);

/**
 * @brief Get the at the element at an index in a linked list
 *
 * @param l The linked list
 * @param index The index at which the data is
 * @return void* A pointer to the data retrieved
 */
void *get_at_index(Linked_list *l, int index);

/**
 * @brief Set the element at the index to the data passed
 *
 * @param l The linked list
 * @param index The index at which the data should be replaced
 * @param data The data that will replace the current data
 */
void set_at_index(Linked_list *l, int index, void *data);

/**
 * @brief Inserts element into a linked list at an index
 *
 * @param l The linked list
 * @param index The index where you want to insert_at_index
 * @param data The data we want to insert_at_index
 */
void insert_at_index(Linked_list *l, int index, void *data);

/**
 * @brief Remove an element as an index from a linked list
 *
 * @param l The linked list
 * @param index The index of the element we want to remove_at_index
 * @return void* A pointer to the data removed
 */
void *remove_at_index(Linked_list *l, int index);

/**
 * @brief Gets the length of a linked list
 *
 * @param l The linked list
 * @return int The length of the list
 */
int ll_len(Linked_list *l);

#endif