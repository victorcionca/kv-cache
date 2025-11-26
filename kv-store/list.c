#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List* create_list() {
    List *l = malloc(sizeof(List));
    l->head = NULL;
    l->tail = NULL;

    l->num_entries = 0;
    return l;
}

/* Create a new node */
Node* create_node(void *data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

/* Insert at head */
Node* insert_head(List *list, void *data) {
    Node *new_node = create_node(data);
    if (!new_node) return NULL;
    new_node->next = list->head;
    if (list->head) {
        list->head->prev = new_node;
    }else {
        list->tail = new_node;
    }
    list->head = new_node;

    list->num_entries ++;

    return new_node;
}

/* Insert at tail */
Node* insert_tail(List *list, void *data) {
    Node *new_node = create_node(data);
    if (!new_node) return NULL;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
        return new_node;
    }
    new_node->prev = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;
    list->num_entries ++;

    return new_node;
}

Node* find_node(List *list, void *data, int (*cmp)(void *, void*)) {
    if (!list || !list->head || !data) return NULL;

    Node *temp = list->head;
    while (temp){
        if (cmp(temp->data, data)) return temp;
        temp = temp->next;
    }

    return NULL;
}

/* Delete a node */
void delete_node(List *list, Node *node) {
    if (!list || !list->head || !node) return;

    if (list->head == node)
        list->head = node->next;
    if (list->tail == node)
        list->tail = node->prev;

    if (node->prev)
        node->prev->next = node->next;

    if (node->next)
        node->next->prev = node->prev;

    list->num_entries --;

    free(node);
}

int node_replace_data(Node *node, void *data) {
    if (!node || !data) return -1;
    free(node->data);
    node->data = data;
    return 0;
}

/* Print list using callback to format data */
void print_list(List *list, void (*print_func)(void *)) {
    Node *temp = list->head;
    while (temp) {
        print_func(temp->data);
        temp = temp->next;
    }
}

/* Free entire list */
void free_list(List *list) {
    Node *temp = list->head;
    while (temp) {
        Node *next = temp->next;
        free(temp);
        temp = next;
    }
    free(list);
}

/* Move a node to the head of the list */
void move_to_head(List *list, Node *node) {
    if (!list || !list->head || !node) return;

    if (node == list->head) return;

    Node *nprev = node->prev;
    Node *nnext = node->next;

    nprev->next = nnext;
    if (node == list->tail) list->tail = nprev;
    else nnext->prev = nprev;

    node->next = list->head;
    node->prev = NULL;
    list->head->prev = node;

    list->head = node;
}
