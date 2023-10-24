#include "stack.h"

int main() {
    stack ststststst = stack_init();
    STACK_DUMP(&ststststst);

    stack_push(&ststststst, 5);
    stack_push(&ststststst, 6);

    STACK_DUMP(&ststststst);

    stack_push(&ststststst, 7);
    stack_push(&ststststst, 8);
    stack_push(&ststststst, 9);
    stack_push(&ststststst, 10);
    stack_push(&ststststst, 11);
    stack_push(&ststststst, 12);
    stack_push(&ststststst, 13);

    STACK_DUMP(&ststststst);
    
    stack_pop(&ststststst);
    stack_pop(&ststststst);
    stack_pop(&ststststst);
    stack_pop(&ststststst);
    stack_pop(&ststststst);
    stack_pop(&ststststst);
    stack_pop(&ststststst);

    STACK_DUMP(&ststststst);

    stack_push(&ststststst, 1337);

    STACK_DUMP(&ststststst);


    stack_destroy(&ststststst);
    return 0;
}

