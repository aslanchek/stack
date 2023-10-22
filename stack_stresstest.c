#include "stack.h"

#undef  VERBOSE
#define FAST

int main() {
    stack SSS = stack_init();

    for (size_t i = 0; i < 200000000; i++) {
        stack_push(&SSS, 1337);
    }

    for (size_t i = 0; i < 200000000; i++) {
        volatile int tmp = stack_top(&SSS);
        stack_pop(&SSS);
    }

    stack_destroy(&SSS);
    return 0;
}

