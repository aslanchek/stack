#include "stack.c"

DEFINE_STACK(int, "%d")

typedef struct {
    char buffer[64];
    stack_int ststststst;
} AAAAAAA;


int main() {

    stack_int stk = stack_int_init();

    stack_int_push(&stk, 5);
    stack_int_push(&stk, 6);
    stack_int_push(&stk, 7);

    STACK_DUMP(&stk, int);

    //char *addr = (char *) stk.__data_cnr1;
    //*addr = 0xbb;
    stk.__arr = NULL;

    stack_int_push(&stk, 8);

    stack_int_destroy(&stk);
    return 0;
}

