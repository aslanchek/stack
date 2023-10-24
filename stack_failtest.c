#include "stack.h"

int main() {
    stack ststststst = stack_init();

    stack_push(&ststststst, 5);
    stack_push(&ststststst, 6);
    stack_push(&ststststst, 7);

    STACK_DUMP(&ststststst);

    stack_pop(&ststststst);

    STACK_DUMP(&ststststst);

    ststststst.__arr = NULL;

    stack_push(&ststststst, 1337);

    stack_destroy(&ststststst);
    return 0;
}

