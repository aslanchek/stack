/*
 *
 * STACK
 *
 * NODE BASED STACK:
 * stack = { capacity, size, _head_node }
 *                              |
 *                              |
 *                              V
 *                           _node = { _next_node, data = [ ******** ] }
 *                                          |
 *                                          V
 *                                        _node = { _next_node, data = [ ******** ] }
 *                                                       |
 *                                                       V
 *                                                     _node = { _next_node, data = [ ******** ] }
 *                                                                                  <---------->
 *                                                                                 node data size = 64 bytes
 *
 *
 * TODO:
   + Constructor
   + Destructor
   * Validator
   + Dump
    
     element access:
     + top()
    
     modifiers:
     + push()
     + pop()
    
     capacity:
     + empty()
     + size()
     
  
   feautes:
   * marco codogen
   * custom allocator
   * stack allocator
 *
 */

#include <stdlib.h> // calloc, realloc, size_t

#ifndef FAST
#include <string.h> // memset
#endif

#ifdef VERBOSE
#include <stdio.h> // fprintf
#endif

typedef int TYPE;

typedef struct {
    size_t __capacity;
    size_t __size;

    TYPE *__arr;

} stack;

stack stack_init() {
    return (stack) {
        .__capacity = 2,
        .__size = 0,
        .__arr = calloc(2, sizeof(TYPE)),
    };
}

void stack_destroy(stack *stk) {
    stk->__capacity = 0;
    stk->__size = 0;
    free(stk->__arr);
    stk->__arr = NULL;
}
/*
 * #
 * __FILE_NAME__
 * __LINE__
 * __FUNCTION__ (__PRETTY_FUNCTION__ or __func__)
 *
 ***************************************************
 * ---STACK DUMP---
 * 
 * Called from stack_unittest.c:22 (int main())
 * 
 * struct stack [0x7fff7ca947f0] "&SSS" {
 *     capacity = 6
 *     size     = 3
 *     data [0x564a968532a0]
 *     {
 *       *[0] = 5
 *       *[1] = 6
 *       *[2] = 7
 *        [3] = 8 (invalid)
 *        [4] = 9 (invalid)
 *        [5] = 0 (invalid)
 *     }
 * }
 **************************************************
 * TODO
 * ---STACK DUMP---
 * 
 * Called from stack_unittest.c:22 (int main())
 * 
 * struct stack [0x7fff7ca947f0] "&SSS" {
 *     capacity = 100
 *     size     = 55
 *     data [0x564a968532a0]
 *     {
 *       *[0] = 5
 *       *[1] = 6
 *       *[2] = 7
 *       *[3] = 8
 *       *[4] = 8
 *       *[5] = 8
 *       *[6] = 8
 *       *[7] = 8
 *       *[8] = 8
 *       *[9] = 8
 *        ...
 *     }
 * }
 **************************************************
 * TODO
 * ---STACK DUMP---
 * 
 * Called from stack_unittest.c:22 (int main())
 * 
 * struct stack [0x7fff7ca947f0] "&SSS" {
 *     capacity = 100
 *     size     = 1
 *     data [0x564a968532a0]
 *     {
 *       *[0] = 5
 *        [1] = -9999 (invalid)
 *        [2] = -9999 (invalid)
 *        [3] = -9999 (invalid)
 *        [4] = -9999 (invalid)
 *        [5] = -9999 (invalid)
 *        [6] = -9999 (invalid)
 *        [7] = -9999 (invalid)
 *        [8] = -9999 (invalid)
 *        [9] = -9999 (invalid)
 *        ...
 *     }
 * }*
 */
static void _stack_dump(stack *stk, const  char *objname, const char *filename, const size_t line, const char *funcname) {
#ifdef VERBOSE
    
    fprintf(stderr, 
        "---STACK DUMP---\n\n"
        "Called from %s:%zu (%s)\n\n"  // filename, line, funcname
        "struct stack [%p] \"%s\" {\n"        // stack addr, object varname
        "  capacity = %zu\n"         // stack capacity
        "  size     = %zu\n\n"       // stack size
        "  data [%p] {\n",           // stack data pointer
        filename, line, funcname, stk, objname, stk->__capacity, stk->__size, stk->__arr);

    for (size_t i = 0; i < stk->__size; i++) {
        fprintf(stderr, "    *[%zu] = %d\n", i, stk->__arr[i]);
    }

    for (size_t i = stk->__size; i < stk->__capacity; i++) {
        fprintf(stderr, "     [%zu] = %d (invalid)\n", i, stk->__arr[i]);
    }

    fprintf(stderr, "  }\n}\n");

#endif
}

#ifdef VERBOSE
#define stack_dump(stk) _stack_dump(stk, #stk+1, __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)
#endif

static int stack_empty(stack *stk) {
    return stk->__size == 0;
}

static size_t stack_size(stack *stk) {
    return stk->__size;
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

    stk->__arr = reallocarray(stk->__arr, newsize, sizeof(TYPE)); // alocator_reallocarray(str->__arr, newsize, sizeof(TYPE));

    #ifndef FAST
    memset(stk->__arr + stk->__size, 0, (newsize - stk->__size) * sizeof(TYPE));
    #endif

    stk->__capacity = newsize;
}

static void stack_push(stack *stk, TYPE val) {
    // asserts
    // validation

    #ifdef VERBOSE
    fprintf(stderr, "---STACK LOG---\npush %d\n", val);
    #endif

    if (stk->__size == stk->__capacity) {
        stack_resize(stk, stk->__capacity * 2);
    }

    stk->__arr[stk->__size++] = val; // allocator_construct(stk->__arr + stk->__size++, val);

    return /* status code */;
};

static void stack_pop(stack *stk) {

    #ifdef VERBOSE
    fprintf(stderr, "---STACK LOG---\npop\n");
    #endif

    stk->__size--;
}

static TYPE stack_top(stack *stk) {
    // asserts:
    //    stack arr ptr is not null
    //    stack size < stack capacity;
    //    
    // validation:
    //    _stack_validate(stk);

    #ifdef VERBOSE
    fprintf(stderr, "---STACK LOG---\ntop -> %d\n", stk->__arr[stk->__size]);
    #endif

    return stk->__arr[stk->__size];
}

