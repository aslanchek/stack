#include <stack.h>

DEFINE_STACK(int, "%d")

int main() {

    stack_int stk = stack_int_init();

    stack_int_push(&stk, 5, LOGINFO);
    stack_int_push(&stk, 6, LOGINFO);
    stack_int_push(&stk, 7, LOGINFO);

    STACK_DUMP(&stk, int);

    char *addr = (char *) stk.__data_cnr1;
    *addr = 0xbb;

    stack_int_push(&stk, 8, LOGINFO);

    stack_int_destroy(&stk);
    return 0;
}

