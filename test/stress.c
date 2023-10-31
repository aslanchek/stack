#include <stack.h>

DEFINE_STACK(size_t, "zu")

int main() {
    stack_size_t SSS = stack_size_t_init();

    for (size_t i = 0; i < 200000000; i++) {
        stack_size_t_push(&SSS, i);
    }

    for (size_t i = 0; i < 200000000; i++) {
        volatile int tmp = stack_size_t_top(&SSS);
        stack_size_t_pop(&SSS);
    }

    stack_size_t_destroy(&SSS);
    return 0;
}

