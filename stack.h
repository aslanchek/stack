/*
 *
 * STACK
 *
 * TODO:
   * Constructor
   * Destructor
   * Validator
   * Dump
   *
   * element access:
     * top()
   *
   * modifiers:
     * push()
     * pop()
   *
   * capacity:
     * empty()
     * size()
   * 
 *
 * feautes:
   * marco codogen
   * custom allocator
   * stack allocator
 *
 */

#include <stdlib.h>

// TODO: wrap into VERBOSE
#include <stdio.h>
//

typedef int TYPE;

typedef struct {
    size_t capacity;
    size_t size;

    TYPE *arr;

} stack;

stack stack_init() {
    return (stack) { .capacity = 2, .size = 0, .arr = calloc(2, sizeof(TYPE)) };
}

void stack_destroy(stack *stk) {
    stk->capacity = 0;
    stk->size = 0;
    free(stk->arr);
    stk->arr = NULL;
}



static void stack_resize(stack *stk, size_t newsize) {
    // asserts:
    //    stack arr ptr is not null
    //    stack size < stack capacity;
    //    
    // validation:
    //    _stack_validate(stk);

    #ifdef VERBOSE
    fprintf(stderr, "---STACK LOG---\nresize to %zu\n", newsize);
    #endif

    stk->arr = reallocarray(stk->arr, newsize, sizeof(TYPE)); // alocator_reallocarray(str->arr, newsize, sizeof(TYPE));
    stk->capacity = newsize;
}

static void stack_push(stack *stk, TYPE val) {
    // asserts
    // validation

    #ifdef VERBOSE
    fprintf(stderr, "---STACK LOG---\npush %d\n", val);
    #endif

    if (stk->size == stk->capacity) {
        stack_resize(stk, stk->capacity * 2);
    }

    stk->arr[stk->size++] = val; // allocator_construct(stk->arr + stk->size++, val);

    return /* status code */;
};

static void stack_pop(stack *stk) {

    #ifdef VERBOSE
    fprintf(stderr, "---STACK LOG---\npop\n");
    #endif

    stk->size--;
}

static TYPE stack_top(stack *stk) {
    // asserts:
    //    stack arr ptr is not null
    //    stack size < stack capacity;
    //    
    // validation:
    //    _stack_validate(stk);

    #ifdef VERBOSE
    fprintf(stderr, "---STACK LOG---\ntop -> %d\n", stk->arr[stk->size]);
    #endif

    return stk->arr[stk->size];
}

static void stack_dump(stack *stk) {
    fprintf(stderr, "---STACK DUMP---\n"
                    "stack {\n"
                    "  capacity = %zu\n"
                    "  size     = %zu\n",
                    stk->capacity,
                    stk->size);
    fprintf(stderr, "  data {\n");
    for (size_t i = 0; i < stk->size; i++) {
        fprintf(stderr, "    *[%zu] = %d\n", i, stk->arr[i]);
    }

    for (size_t i = stk->size; i < stk->capacity; i++) {
        fprintf(stderr, "     [%zu] = %d\n", i, stk->arr[i]);
    }
    fprintf(stderr, "  }\n}\n");
}

