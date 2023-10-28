#include "stack.h"

DEFINE_STACK(int)

int main() {
    stack_int ststststst = stack_init();

    stack_push(&ststststst, 5);
    stack_push(&ststststst, 6);

    stack_push(&ststststst, 7);
    stack_push(&ststststst, 8);
    stack_push(&ststststst, 9);
    
    stack_pop(&ststststst);
    stack_pop(&ststststst);

    stack_push(&ststststst, 1337);

    stack_destroy(&ststststst);
    return 0;
}

