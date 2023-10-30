#include "stack.c"

DEFINE_STACK(int, "%d")    // -> stack_int

DEFINE_STACK(float, "%f")  // -> stack_float

int main() {
    // -------------INT-------------
    stack_int stk_i = stack_int_init();

    stack_int_push(&stk_i, 5);
    stack_int_push(&stk_i, 6);
    stack_int_push(&stk_i, 7);
    stack_int_push(&stk_i, 8);
    stack_int_push(&stk_i, 9);

    STACK_DUMP(&stk_i, int);
    
    stack_int_pop(&stk_i);
    stack_int_pop(&stk_i);

    STACK_DUMP(&stk_i, int);

    stack_int_push(&stk_i, 1337);

    STACK_DUMP(&stk_i, int);

    stack_int_empty(&stk_i);
    stack_int_size(&stk_i);
    stack_int_top(&stk_i);

    stack_int_destroy(&stk_i);

    // ----------FLOAT-------------
    stack_float stk_f = stack_float_init();

    stack_float_push(&stk_f, 5.5);
    stack_float_push(&stk_f, 6.1);

    STACK_DUMP(&stk_f, float);

    stack_float_push(&stk_f, 3.141528);
    stack_float_push(&stk_f, 2.718025);
    stack_float_push(&stk_f, 9.999);

    stack_float_top(&stk_f);


    STACK_DUMP(&stk_f, float);

    stack_float_destroy(&stk_f);

    return 0;
}

