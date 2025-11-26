#ifndef __LIST_H_
#define __LIST_H_

/* Doubly linked list node */
typedef struct Node {
    void *data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    unsigned int num_entries;
} List;

List* create_list();
Node* create_node(void *data);
Node* insert_head(List *list, void *data);
Node* insert_tail(List *list, void *data);

/*
 * Find a node containing data.
 * cmp is a function that compares two data, returns 0 on match, 1 otherwise
 */
Node* find_node(List *list, void *data, int (*cmp)(void *, void*));
void delete_node(List *list, Node *node);
/* Replace node data, freeing previous one. Returns !0 if node or data is null */
int node_replace_data(Node *node, void *new_data);
/* Print the list by providing a function that prints every node */
void print_list(List *list, void (*print_func)(void *));
void free_list(List *list);
void move_to_head(List *list, Node *node);
#endif
