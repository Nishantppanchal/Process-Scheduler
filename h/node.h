// Prevent multiple declarations when included in multiple times
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

/* Node structure */
struct Node
{
    void *data; // A pointer to the data linked to the node
    struct Node *next; // A pointer to the next node
};
typedef struct Node Node; // Creates a type Node defining it as the Node structure

/**
 * @brief Create a Node object
 * 
 * @param data The data to be stored by the node
 * @param next A pointer to the next node in the linked list
 * @return Node* A pointer to the newly created node
 */
Node *create_node(void *data, Node *next);

/**
 * @brief Edits the data of a node
 * 
 * @param new_data The new data that we want to replace the existing data with
 * @param node A pointer to the node
 */
void edit_node(void *new_data, Node *node);

/**
 * @brief Deletes a node
 * 
 * @param node The node to be deleted
 * @return void* The data stored by the node
 */
void *delete_node(Node *node);

#endif