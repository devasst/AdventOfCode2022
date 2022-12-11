typedef struct stack_node_s stack_node_t;
typedef struct stack_s stack_t;

typedef void (*stack_destroy_value_cb)(void *value);

stack_t *stack_new(stack_destroy_value_cb destroy_value_cb);
void stack_destroy(stack_t *self);

int stack_push_node(stack_t *self, void *value);
stack_node_t *stack_pop_node(stack_t *self);
void *stack_node_get_value(stack_node_t *self);
void stack_destroy_node(stack_t *self, stack_node_t *node);