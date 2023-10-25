/*
 *
 * STACK
 *
 * NODE BASED STACK:
 * stack = { capacity, size, _head_node }
 *                             |
 *                             |
 *                             V
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
   feautes:
   * marco codogen
   * custom allocator
   * stack allocator
 *
 */

/*
 * compilation options
 *   NOMEMZERO - no not perform new allocated memory zeroing
 *   USEDUMP   - enables stack dump facilities
 *   VERBOSE   - enables verbosibility in stack methods
 *   VALIDATE  - perform stack integrity validation per each method
 *
 */

#include <stdlib.h> // calloc, realloc, size_t

#ifndef NOMEMZERO
#include <string.h> // memset
#endif

#include <assert.h> // assert

#if defined USEDUMP || defined VERBOSE
#include <stdio.h>    // fprintf
#include <execinfo.h> // backtrace
#endif


#define ANSI_COLOR_RED  "\x1b[31m"
#define ANSI_COLOR_YLW  "\x1b[33m"
#define ANSI_COLOR_RST  "\x1b[0m"

#define RED(str) ANSI_COLOR_RED str ANSI_COLOR_RST

typedef int TYPE;
#define TYPE_FORMAT "%d"

typedef struct {
    size_t __cpct;
    size_t __size;

    TYPE *__arr;

} stack;


__attribute__((weak))
stack stack_init() {
    return (stack) {
        .__cpct = 2,
        .__size = 0,
        .__arr = calloc(2, sizeof(TYPE)),
    };
}


__attribute__((weak))
void stack_destroy(stack *stk) {
    assert(stk);
    assert(stk->__arr);

    stk->__cpct = 0;
    stk->__size = 0;
    free(stk->__arr);
    stk->__arr = NULL;
}


// returns 0 on valid integrity
__attribute__((weak))
int _stack_validate(stack *stk) {
    // ifndef NDEBUG
    assert(stk);
    assert(stk->__arr);
    // endif

    if (!stk) {
        // set the stack_errno
        return 1;
    }

    if (!stk->__arr) {
        // set the stack_errno
        return 2/*invalid data pointer*/;
    }

    if (stk->__cpct == 0) {
        // set the stack_errno
        return 3/*zero capacity*/;
    }

    if (stk->__cpct < stk->__size) {
        // set the stack_errno
        return 4/*invalid size-capacity relation*/;
    }

    // if canary is alive ...

    return 0; // stack is valid
}


__attribute__((weak))
void _stack_fail(stack *stk,
            const int status,
            const char *methodname,
            const char *filename,
            const size_t line,
            const char *funcname) {

    // if VERBOSE is not defined, show nothing.
    #ifdef VERBOSE

    fprintf(stderr, 
         RED("[stack fail]")" %s:%zu in (%s): Validation failed: %s status code: %d\n",
         filename, line, funcname, methodname, status);

    // stack memory dump

    #endif

    abort();
}
#define STACK_FAIL(stk, status) _stack_fail(stk, status, "undefined", __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


__attribute__((weak))
void _stack_dump(stack *stk,
            const  char *objname,
            const char *filename,
            const size_t line,
            const char *funcname) {

    // if USEDUMP is not defined, show nothing.
    #ifdef USEDUMP

    _stack_validate(stk);
    
    fprintf(stderr, 
        "[stack log] called from %s:%zu in (%s): stack dump\n"         // filename, line, funcname
        "struct [%p] \"%s\" {\n"                          // stack addr, object varname
        "  capacity = %zu\n"                         // stack capacity
        "  size     = %zu\n"                         // stack size
        "  data [%p] {\n",                           // stack data pointer
        filename, line, funcname, stk, objname, stk->__cpct, stk->__size, stk->__arr);

    for (size_t i = 0; i < stk->__size; i++) {
        fprintf(stderr, "    *[%zu] = "TYPE_FORMAT"\n", i, stk->__arr[i]);
    }

    for (size_t i = stk->__size; i < stk->__cpct; i++) {
        fprintf(stderr, "     [%zu] = "TYPE_FORMAT"\n", i, stk->__arr[i]);
    }

    fprintf(stderr, "  }\n}\n");

    #endif
}
#define STACK_DUMP(stk) _stack_dump(stk, (#stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


__attribute__((weak))
int _stack_empty(stack *stk, const char *filename, size_t fileline, const char *funcname) {
    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        STACK_FAIL(stk, vcode);
    }
    #endif

    #ifdef VERBOSE
    fprintf(stderr, "[stack log] called from %s:%zu in (%s): stack_empty() -> %d\n", filename, fileline, funcname, stk->__size == 0);
    #endif

    return stk->__size == 0;
}
#define stack_empty(stk) _stack_empty((stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)

__attribute__((weak))
size_t _stack_size(stack *stk, const char *filename, size_t fileline, const char *funcname) {
    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        _stack_fail(stk, vcode, "stack_size()", filename, fileline, funcname);
    }
    #endif

    #ifdef VERBOSE
    fprintf(stderr, "[stack log] called from %s:%zu in (%s): stack_size() -> %zu\n", filename, fileline, funcname, stk->__size);
    #endif

    return stk->__size;
}
#define stack_size(stk) _stack_size((stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)

__attribute__((weak))
void _stack_resize(stack *stk, size_t newsize,
        const char *filename,
        size_t fileline,
        const char *funcname) {

    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        STACK_FAIL(stk, vcode);
    }
    #endif

    #ifdef VERBOSE
    fprintf(stderr, "[stack log] called from %s:%zu in (%s): stack_resize(%zu)\n", filename, fileline, funcname, newsize);
    #endif

    // alocator_reallocarray(str->__arr, newsize, sizeof(TYPE));
    stk->__arr = reallocarray(stk->__arr, newsize, sizeof(TYPE));

    #ifndef NOMEMZERO
    memset(stk->__arr + stk->__size, 0, (newsize - stk->__size) * sizeof(TYPE));
    #endif

    stk->__cpct = newsize;
}
#define stack_resize(stk, newsize) _stack_resize((stk), (newsize), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


__attribute__((weak))
void _stack_push(stack *stk, TYPE val,
        const char *filename,
        size_t fileline,
        const char *funcname) {

    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        _stack_fail(stk, vcode, "stack_push()", filename, fileline, funcname);
    }
    #endif

    #ifdef VERBOSE
    fprintf(stderr, "[stack log] called from %s:%zu in (%s): stack_push("TYPE_FORMAT")\n", filename, fileline, funcname, val);
    #endif

    if (stk->__size == stk->__cpct) {
        stack_resize(stk, stk->__cpct * 2);
    }

    // allocator_construct(stk->__arr + stk->__size++, val);
    stk->__arr[stk->__size++] = val;

    return /* status code */;
};
#define stack_push(stk, val) _stack_push((stk), (val), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


__attribute__((weak))
void _stack_pop(stack *stk, const char *filename, size_t fileline, const char *funcname) {
    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        _stack_fail(stk, vcode, "stack_pop()", filename, fileline, funcname);
    }
    if (stk->__size == 0) {
        _stack_fail(stk, /*pop empty stack*/5, "stack_pop()", filename, fileline, funcname);
    }
    #endif

    #ifdef VERBOSE
    fprintf(stderr, "[stack log] called from %s:%zu in (%s): stack_pop()\n", filename, fileline, funcname);
    #endif

    if (stk->__size * 2 < stk->__cpct) {
      stack_resize(stk, stk->__cpct / 2);
    }

    stk->__size--;
}
#define stack_pop(stk) _stack_pop((stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


__attribute__((weak))
TYPE _stack_top(stack *stk, const char *filename, size_t fileline, const char *funcname) {
    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        _stack_fail(stk, vcode, "stack_top()", filename, fileline, funcname);
    }
    if (stk->__size == 0) {
        _stack_fail(stk, /*accessing empty stack*/6, "stack_top()", filename, fileline, funcname);
    }
    #endif

    #ifdef VERBOSE
    fprintf(stderr, "[stack log] call from %s:%zu in (%s): top() -> "TYPE_FORMAT"\n", filename, fileline, funcname, stk->__arr[stk->__size]);
    #endif

    return stk->__arr[stk->__size];
}
#define stack_top(stk) _stack_top((stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)

