#include "stack.h"

int main() {
    stack ststststst = stack_init();

    stack_dump(&ststststst);

    stack_push(&ststststst, 5);
    stack_push(&ststststst, 6);

    stack_dump(&ststststst);

    stack_push(&ststststst, 7);
    stack_push(&ststststst, 8);
    stack_push(&ststststst, 9);

    stack_dump(&ststststst);
    
    stack_pop(&ststststst);
    stack_pop(&ststststst);

    stack_dump(&ststststst);

    stack_push(&ststststst, 1337);

    stack_dump(&ststststst);


    stack_destroy(&ststststst);
    return 0;
}

