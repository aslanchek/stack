/*
 * compile options
 *   NOMEMZERO - no not perform new allocated memory zeroing
 *   USEDUMP   - enables stack dump facilities
 *   VERBOSE   - enables verbosibility in stack methods
 *   VALIDATE  - perform stack integrity validation per each method
 *   NDEBUG    - ignore all asserts from <assert.h>
 *   CANARY    - use buffer overflow protection with canaries
 *
 */

#include <stdlib.h> // calloc, realloc, size_t

#ifndef NOMEMZERO // TODO: substitute this
#include <string.h> // memset
#endif

#include <assert.h> // assert

#if defined USEDUMP || defined VERBOSE
#include <stdio.h> // fprintf
#endif

#define ANSI_COLOR_RED  "\x1b[31m"
#define ANSI_COLOR_YLW  "\x1b[33m"
#define ANSI_COLOR_RST  "\x1b[0m"

#define RED(str) ANSI_COLOR_RED str ANSI_COLOR_RST

typedef int TYPE;
#define TYPE_FORMAT "%d"

#define CANARY

#ifdef CANARY
#define STATIC_CANARY_VAL 0x0a0a0a0a0a0a0a0au
typedef size_t CANARY_TYPE;
#endif


typedef struct {
    #ifdef CANARY
    //const CANARY_TYPE* __strct_cnr1;
    const CANARY_TYPE* __data_cnr1;
    const CANARY_TYPE* __data_cnr2;
    #endif

    /*
    #ifdef HASH
    uint32_t hash;
    #endif
    */

    size_t __cpct;
    size_t __size;

    TYPE *__arr;

    #ifdef CANARY
    //const CANARY_TYPE* __strct_cnr2;
    #endif
} stack;


__attribute__((weak))
stack stack_init() {
    stack newstk = (stack) {
        .__cpct = 2,
        .__size = 0,
        .__arr = NULL,
    };
                                                         //  / alignment issues
    #ifdef CANARY                                        // v
    newstk.__arr = (TYPE *) (   (char *)malloc( ( (newstk.__cpct * sizeof(TYPE) - 1)/8+1 )*8 + 2 * sizeof(CANARY_TYPE) ) + sizeof(CANARY_TYPE)   );

    newstk.__data_cnr1 = (CANARY_TYPE *) ( (char *)newstk.__arr - sizeof(CANARY_TYPE) );
    newstk.__data_cnr2 = (CANARY_TYPE *) ( (char *)newstk.__arr + ( (newstk.__cpct * sizeof(TYPE) - 1)/8+1 )*8 );

    // safe const assignment
    CANARY_TYPE *tmp1 = (CANARY_TYPE *)newstk.__data_cnr1;
    *tmp1 = STATIC_CANARY_VAL;
    CANARY_TYPE *tmp2 = (CANARY_TYPE *)newstk.__data_cnr2;
    *tmp2 = STATIC_CANARY_VAL;
    //

    #else
    newstk.__arr = malloc( newstk.__cpct * sizeof(TYPE) );
    #endif
    // TODO: what if malloc failed?

    return newstk;
}


__attribute__((weak))
void stack_destroy(stack *stk) {
    assert(stk);
    assert(stk->__arr);

    stk->__cpct = 0;
    stk->__size = 0;

    #ifdef CANARY
    free((char *)stk->__arr - sizeof(CANARY_TYPE));
    #else
    free(stk->__arr);
    #endif
    
    stk->__arr = NULL;
}


#ifdef CANARY
int _stack_canary_invalid(const CANARY_TYPE *ptr) {
    return *ptr != STATIC_CANARY_VAL;
}
#endif


// returns 0 on valid integrity
__attribute__((weak))
int _stack_validate(stack *stk) {
    // ifndef NDEBUG
    assert(stk);
    assert(stk->__arr);
    //TODO: duplicate if statements below with asserts
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

    #ifdef CANARY
    if (!stk->__data_cnr1) {
        return 6/*left canary invalidated*/;
    }

    if (!stk->__data_cnr2) {
        return 6/*left canary invalidated*/;
    }

    if (_stack_canary_invalid(stk->__data_cnr1)) {
        return 7/*buffer overflow attack detected: left canary invaded*/;
    }

    if (_stack_canary_invalid(stk->__data_cnr2)) {
        return 8/*buffer overflow attack detected: right canary invaded*/;
    }
    #endif

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

    #endif //VERBOSE

    abort();
}
#define STACK_FAIL(stk, status) _stack_fail(stk, status, "undefined", __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)



#define LOG_PATTERN  "[stack log] %s:%zu in (%s): "
#define LOG_RETURNS  " -> "

__attribute__((weak))
void _stack_dump(stack *stk,
            const  char *objname,
            const char *filename,
            const size_t line,
            const char *funcname) {

    // if USEDUMP is not defined, show nothing.
    #ifdef USEDUMP

    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        _stack_fail(stk, vcode, "stack_dump()", filename, line, funcname);
    }
    #endif //VALIDATE

    fprintf(stderr, LOG_PATTERN "stack_dump()\n", filename, line, funcname);
    
    fprintf(stderr, "struct [%p] \"%s\" {\n", stk, objname);

    fprintf(stderr,
        "  capacity = %zu\n"
        "  size     = %zu\n"
        "  data [%p] {\n",
          stk->__cpct, stk->__size, stk->__arr);

    #ifdef CANARY
    fprintf(stderr, "    canary1 = %s\n", _stack_canary_invalid(stk->__data_cnr2) ? "failed" : "ok");
    #endif

    for (size_t i = 0; i < stk->__size; i++) {
        fprintf(stderr, "    *[%zu] = "TYPE_FORMAT"\n", i, stk->__arr[i]);
    }

    for (size_t i = stk->__size; i < stk->__cpct; i++) {
        fprintf(stderr, "     [%zu] = "TYPE_FORMAT"\n", i, stk->__arr[i]);
    }

    #ifdef CANARY
    fprintf(stderr, "    canary2 = %s\n", _stack_canary_invalid(stk->__data_cnr2) ? "failed" : "ok");
    #endif

    fprintf(stderr, "  }\n");

    fprintf(stderr, "}\n");

    #endif //USEDUMP
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
    fprintf(stderr, LOG_PATTERN "stack_empty()" LOG_RETURNS "%d" "\n", filename, fileline, funcname, stk->__size == 0);
    #endif //VERBOSE

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
    #endif //VALIDATE

    #ifdef VERBOSE
    fprintf(stderr, LOG_PATTERN "stack_size()" LOG_RETURNS "%zu" "\n", filename, fileline, funcname, stk->__size);
    #endif //VERBOSE

    return stk->__size;
}
#define stack_size(stk) _stack_size((stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


__attribute__((weak))
void _stack_resize(stack *stk, size_t newcpct,
        const char *filename,
        size_t fileline,
        const char *funcname) {

    #ifdef VALIDATE
    int vcode = _stack_validate(stk);
    if (vcode) {
        STACK_FAIL(stk, vcode);
    }
    #endif //VALIDATE

    #ifdef VERBOSE
    fprintf(stderr, LOG_PATTERN "stack_resize(%zu)\n", filename, fileline, funcname, newcpct);
    #endif //VERBOSE

    stk->__cpct = newcpct;

    #ifdef CANARY
    stk->__arr = (TYPE *) (   (char *)realloc((char *)stk->__arr - sizeof(CANARY_TYPE), ( (newcpct * sizeof(TYPE) - 1)/8+1 )*8 + 2 * sizeof(CANARY_TYPE)) + sizeof(CANARY_TYPE)   );

    stk->__data_cnr1 = (CANARY_TYPE *) ( (char *)stk->__arr - sizeof(CANARY_TYPE) );
    stk->__data_cnr2 = (CANARY_TYPE *) ( (char *)stk->__arr + ((stk->__cpct * sizeof(TYPE) - 1)/8+1 )*8 );

    // safe const assignment
    CANARY_TYPE *tmp1 = (CANARY_TYPE *)stk->__data_cnr1;
    *tmp1 = STATIC_CANARY_VAL;
    CANARY_TYPE *tmp2 = (CANARY_TYPE *)stk->__data_cnr2;
    *tmp2 = STATIC_CANARY_VAL;
    //

    #else
    stk->__arr = reallocarray(stk->__arr, newcpct, sizeof(TYPE));
    #endif

    #ifndef NOMEMZERO
    memset(stk->__arr + stk->__size, 0, (newcpct - stk->__size) * sizeof(TYPE));
    #endif
}
#define stack_resize(stk, newcpct) _stack_resize((stk), (newcpct), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


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
    fprintf(stderr, LOG_PATTERN "stack_push("TYPE_FORMAT")\n", filename, fileline, funcname, val);
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
    fprintf(stderr, LOG_PATTERN "stack_pop()\n", filename, fileline, funcname);
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
    fprintf(stderr, LOG_PATTERN "stack_top()" LOG_RETURNS TYPE_FORMAT "\n", filename, fileline, funcname, stk->__arr[stk->__size]);
    #endif

    return stk->__arr[stk->__size];
}
#define stack_top(stk) _stack_top((stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)

