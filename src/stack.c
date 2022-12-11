
#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"

typedef struct stack_node_s{
    void *value;
    struct stack_node_s *next;
} stack_node_t;

typedef struct stack_s{
    stack_node_t *nodes;
    stack_destroy_value_cb destroy_value_cb;
} stack_t;

stack_t *stack_new(stack_destroy_value_cb destroy_value_cb){
    stack_t *self = (stack_t *)calloc(1, sizeof(stack_t));
    self->destroy_value_cb = destroy_value_cb;
    return self;
}

void stack_destroy(stack_t *self){
    if(self == NULL){
        return;
    }
    stack_node_t *next = self->nodes;
    while(next){
        stack_node_t *tmp = next->next;
        if(self->destroy_value_cb){
            self->destroy_value_cb(next->value);
        }
        free(next);
        next = tmp;
    }
    free(self);
}

int stack_push_node(stack_t *self, void *value){
    if(self == NULL){
        return -1;
    }

    stack_node_t *new_node = (stack_node_t*)calloc(1, sizeof(stack_node_t));
    new_node->value = value;
    new_node->next = self->nodes;
    self->nodes = new_node;
    return 0;
}

stack_node_t *stack_pop_node(stack_t *self){
    if(self == NULL){
        return NULL;
    }

    stack_node_t *poped_node = self->nodes;
    self->nodes = poped_node->next;
    poped_node->next = NULL;
    return poped_node;
}

void *stack_node_get_value(stack_node_t *self){
    if(self == NULL){
        return NULL;
    }
    return self->value;
}

void stack_destroy_node(stack_t *self, stack_node_t *node){
    if (self == NULL || node == NULL){
        return;
    }
    if(self->destroy_value_cb){
        self->destroy_value_cb(node->value);
    }
    free(node);
}