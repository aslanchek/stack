#include <stack.h>

DEFINE_STACK(int, "%d")    // -> stack_int

DEFINE_STACK(float, "%f")  // -> stack_float

int main() {
    // -------------INT-------------
    stack_int stk_i = stack_int_init();

    stack_int_push(&stk_i, 5, LOGINFO);
    stack_int_push(&stk_i, 6, LOGINFO);
    stack_int_push(&stk_i, 7, LOGINFO);
    stack_int_push(&stk_i, 8, LOGINFO);
    stack_int_push(&stk_i, 9, LOGINFO);

    STACK_DUMP(&stk_i, int);
    
    stack_int_pop(&stk_i, LOGINFO);
    stack_int_pop(&stk_i, LOGINFO);

    STACK_DUMP(&stk_i, int);

    stack_int_push(&stk_i, 1337, LOGINFO);

    STACK_DUMP(&stk_i, int);

    stack_int_empty(&stk_i, LOGINFO);
    stack_int_size(&stk_i, LOGINFO);
    stack_int_top(&stk_i, LOGINFO);

    stack_int_destroy(&stk_i);

    // ----------FLOAT-------------
    stack_float stk_f = stack_float_init();

    stack_float_push(&stk_f, 5.5, LOGINFO);
    stack_float_push(&stk_f, 6.1, LOGINFO);

    STACK_DUMP(&stk_f, float);

    stack_float_push(&stk_f, 3.141528, LOGINFO);
    stack_float_push(&stk_f, 2.718025, LOGINFO);
    stack_float_push(&stk_f, 9.999, LOGINFO);

    stack_float_top(&stk_f, LOGINFO);


    STACK_DUMP(&stk_f, float);

    stack_float_destroy(&stk_f);

    return 0;
}

