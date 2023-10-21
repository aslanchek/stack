#include "stack.h"

int main() {
    stack SSS = stack_init();

    stack_dump(&SSS);

    stack_push(&SSS, 5);
    stack_push(&SSS, 6);

    stack_dump(&SSS);

    stack_push(&SSS, 7);
    stack_push(&SSS, 8);
    stack_push(&SSS, 9);

    stack_dump(&SSS);
    
    stack_pop(&SSS);
    stack_pop(&SSS);

    stack_dump(&SSS);

    stack_push(&SSS, 1337);

    stack_dump(&SSS);


    stack_destroy(&SSS);
    return 0;
}
