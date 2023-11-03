#include <stack.h>

DEFINE_STACK(int, "%d")

int main() {

    stack_int stk = stack_int_init();

    stack_int_push(&stk, 5, LOGINFO);
    stack_int_push(&stk, 6, LOGINFO);
    stack_int_push(&stk, 7, LOGINFO);

    STACK_DUMP(&stk, int);

    stk.__size = 0;

    stack_int_pop(&stk, LOGINFO);

    stack_int_destroy(&stk);
    return 0;
}

