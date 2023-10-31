/*
 * compile options
 *   NOMEMZERO - no not perform new allocated memory zeroing
 *   USEDUMP   - enables stack dump facilities
 *   VERBOSE   - enables verbosibility in stack methods
 *   VALIDATE  - perform stack integrity validation per each method
 *   NDEBUG    - ignore all asserts from <assert.h>
 *   CANARY    - use buffer overflow protection with canaries
 *
 * TODO:
 *   DEBUG
 *   HASH
 *   CANARY
 *
 */
#ifndef MACROMAGIC_STACK_H
#define MACROMAGIC_STACK_H

#include <stdlib.h> // calloc, realloc, size_t

#include <assert.h> // assert

#if defined(USEDUMP) || defined(VERBOSE)
#include <stdio.h> // fprintf
#endif

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YLW "\x1b[33m"
#define ANSI_COLOR_RST "\x1b[0m"

#define RED(str) ANSI_COLOR_RED str ANSI_COLOR_RST

#ifdef CANARY
#define VALIDATE
#define STATIC_CANARY_VAL 0x0a0a0a0a0a0a0a0au
typedef size_t CANARY_TYPE;
#define IF_CANARY(...) __VA_ARGS__
#define ELSE_CANARY(...)
#else
#define IF_CANARY(...)
#define ELSE_CANARY(...) __VA_ARGS__
#endif

#ifdef HASH
#define IF_HASH(...) __VA_ARGS__

#define ELSE_HASH(...)
unsigned int MurmurHash2 (char * key, unsigned int len) {
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const unsigned int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;
    unsigned int k = 0;

    while (len >= 4) {
        k  = (unsigned int)data[0];
        k |= (unsigned int)data[1] << 8;
        k |= (unsigned int)data[2] << 16;
        k |= (unsigned int)data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
        case 3:
            h ^= (unsigned int)data[2] << 16;
            __attribute__ ((fallthrough));
        case 2:
            h ^= (unsigned int)data[1] << 8;
            __attribute__ ((fallthrough));
        case 1:
            h ^= (unsigned int)data[0];
            h *= m;
        default:
            break;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
#else  // HASH
#define IF_HASH(...)
#define ELSE_HASH(...) __VA_ARGS__
#endif // HASH

#define DEFINE_STACK_STRUCT(TYPE)\
  typedef struct {\
    IF_CANARY(\
    const CANARY_TYPE *__strct_cnr1;\
    const CANARY_TYPE *__data_cnr1;\
    const CANARY_TYPE *__data_cnr2;\
    )\
    IF_HASH(\
    uint32_t hash;\
    )\
    size_t __cpct;\
    size_t __size;\
    TYPE *__arr;\
    IF_CANARY(\
    const CANARY_TYPE *__strct_cnr2;\
    )\
  } stack_##TYPE;\

#define DEFINE_STACK_INIT(TYPE)\
__attribute__((weak))\
stack_##TYPE stack_##TYPE##_init() {\
    stack_##TYPE newstk = (stack_##TYPE){\
        .__cpct = 2,\
        .__size = 0,\
        .__arr = NULL,\
    };\
    IF_CANARY(\
    newstk.__arr = (TYPE *)((char *)malloc(((newstk.__cpct * sizeof(TYPE) - 1) / 8 + 1) * 8 + 2 * sizeof(CANARY_TYPE)) + sizeof(CANARY_TYPE));\
    newstk.__data_cnr1 = (CANARY_TYPE *)((char *)newstk.__arr - sizeof(CANARY_TYPE));\
    newstk.__data_cnr2 = (CANARY_TYPE *)((char *)newstk.__arr + ((newstk.__cpct * sizeof(TYPE) - 1) / 8 + 1) * 8);\
    CANARY_TYPE *tmp1 = (CANARY_TYPE *)newstk.__data_cnr1;\
    *tmp1 = STATIC_CANARY_VAL;\
    CANARY_TYPE *tmp2 = (CANARY_TYPE *)newstk.__data_cnr2;\
    *tmp2 = STATIC_CANARY_VAL;\
    )\
    ELSE_CANARY(\
    newstk.__arr = malloc(newstk.__cpct * sizeof(TYPE));\
    )\
    return newstk;\
  }\

#define DEFINE_STACK_DESTROY(TYPE)\
__attribute__((weak))\
void stack_##TYPE##_destroy(stack_##TYPE *stk) {\
    assert(stk);\
    assert(stk->__arr);\
    stk->__cpct = 0;\
    stk->__size = 0;\
    IF_CANARY(\
    free((char *)stk->__arr - sizeof(CANARY_TYPE));\
    )\
    ELSE_CANARY(\
    free(stk->__arr);\
    )\
    stk->__arr = NULL;\
}\

IF_CANARY(
int _stack_canary_invalid(const CANARY_TYPE *ptr) {
  return *ptr != STATIC_CANARY_VAL;
})

// returns 0 on valid integrity
#define DEFINE_STACK_VALIDATE(TYPE)\
__attribute__((weak))\
int stack_##TYPE##_validate(stack_##TYPE *stk) {\
    assert(stk);\
    assert(stk->__arr);\
    if (!stk) {\
      return 1;\
    }\
    if (!stk->__arr) {\
      return 2 /*invalid data pointer*/;\
    }\
    if (stk->__cpct == 0) {\
      return 3 /*zero capacity*/;\
    }\
    if (stk->__cpct < stk->__size) {\
      return 4 /*invalid size-capacity relation*/;\
    }\
    IF_CANARY(\
    if (!stk->__data_cnr1) {\
       return 6 /*left canary invalidated*/;\
    } if (!stk->__data_cnr2) {\
        return 6 /*left canary invalidated*/;\
    } if (_stack_canary_invalid(stk->__data_cnr1)) {\
       return 7 /*buffer overflow attack detected: left canary invaded*/;\
    } if (_stack_canary_invalid(stk->__data_cnr2)) {\
       return 8 /*buffer overflow attack detected: right canary invaded*/;  \
    }\
    )\
    return 0 /*stack is valid*/;\
  }\

#ifdef VERBOSE
#define IF_VERBOSE(...) __VA_ARGS__
#define ELSE_VERBOSE(...)
#else
#define IF_VERBOSE(...)
#define ELSE_VERBOSE(...) __VA_ARGS__
#endif

#define DEFINE_STACK_FAIL(TYPE)\
__attribute__((weak))\
void _stack_##TYPE##_fail(\
      stack_##TYPE *stk, const int status, const char *methodname\
      IF_VERBOSE(, const char *filename, const size_t line, const char *funcname)\
    ) {\
    IF_VERBOSE(\
    fprintf(stderr, RED("[stack fail]") " %s:%zu in (%s): Validation "\
                                        "failed: %s status code: %d\n", \
                       filename, line, funcname, methodname, status);\
    )\
    abort();\
  }

#define LOG_PATTERN "[stack log] %s:%zu in (%s): "
#define LOG_RETURNS " -> "

#ifdef USEDUMP
#define IF_USEDUMP(...) __VA_ARGS__
#define ELSE_USEDUMP(...)
#else
#define IF_USEDUMP(...)
#define ELSE_USEDUMP(...) __VA_ARGS__
#endif

#ifdef VALIDATE
#define IF_VALIDATE(...) __VA_ARGS__
#define ELSE_VALIDATE(...)
#else
#define IF_VALIDATE(...)
#define ELSE_VALIDATE(...) __VA_ARGS__
#endif

#define DEFINE_STACK_DUMP(TYPE, TYPE_FORMAT)\
__attribute__((weak))\
void _stack_##TYPE##_dump(\
    stack_##TYPE *stk, const char *objname, const char *filename,\
    const size_t line, const char *funcname) {\
    IF_USEDUMP(\
    IF_VALIDATE(\
    int vcode = stack_##TYPE##_validate(stk);\
    if (vcode) {\
          _stack_##TYPE##_fail(stk, vcode, "stack_dump()" IF_VERBOSE(, filename, line, funcname) );\
    })\
    fprintf(stderr, LOG_PATTERN "stack_dump()\n", filename, line, funcname);\
    fprintf(stderr, "struct [%p] \"%s\" {\n", stk, objname);\
    fprintf(stderr,\
        "  capacity = %zu\n"\
        "  size     = %zu\n"\
        "  data [%p] {\n",\
        stk->__cpct, stk->__size, stk->__arr);\
    IF_CANARY(\
    fprintf(stderr, "    canary1 = %s\n",\
        _stack_canary_invalid(stk->__data_cnr2)\
        ? "failed"\
        : "ok");\
    )\
    for (size_t i = 0; i < stk->__size; i++) {\
        fprintf(stderr, "    *[%zu] = " TYPE_FORMAT "\n", i, stk->__arr[i]);\
    }\
    for (size_t i = stk->__size; i < stk->__cpct; i++) {\
        fprintf(stderr, "     [%zu] = " TYPE_FORMAT "\n", i, stk->__arr[i]);\
    }\
    IF_CANARY(\
    fprintf(stderr, "    canary2 = %s\n",\
        _stack_canary_invalid(stk->__data_cnr2) ? "failed" : "ok");\
    )\
    fprintf(stderr, "  }\n");\
    fprintf(stderr, "}\n");\
    )\
}\


#define LOGINFO __FILE__, __LINE__, __PRETTY_FUNCTION__


#define STACK_DUMP(stk, TYPE)\
_stack_##TYPE##_dump(stk, (#stk), LOGINFO)


#define DEFINE_STACK_EMPTY(TYPE, TYPE_FORMAT)\
__attribute__((weak))\
int stack_##TYPE##_empty(stack_##TYPE *stk\
        IF_VERBOSE(, const char *filename,)\
        IF_VERBOSE(size_t fileline,)\
        IF_VERBOSE(const char *funcname)\
    ) {\
    IF_VALIDATE(\
    int vcode = stack_##TYPE##_validate(stk);\
    if (vcode){\
        _stack_##TYPE##_fail(stk, vcode, "stack_" #TYPE "empty()" IF_VERBOSE(, filename, fileline, funcname) );\
    }\
    )\
    IF_VERBOSE(\
    fprintf(stderr,\
        LOG_PATTERN "stack_" #TYPE "_empty()" LOG_RETURNS "%d" "\n",\
                       filename, fileline, funcname, stk->__size == 0);\
    )\
    return stk->__size == 0;\
}\


#define DEFINE_STACK_SIZE(TYPE, TYPE_FORMAT)\
__attribute__((weak))\
size_t stack_##TYPE##_size(stack_##TYPE *stk\
        IF_VERBOSE(, const char *filename,)\
        IF_VERBOSE(size_t fileline,)\
        IF_VERBOSE(const char *funcname)\
    ) {\
    IF_VALIDATE(\
    int vcode = stack_##TYPE##_validate(stk);\
    if (vcode) {\
        _stack_##TYPE##_fail(stk, vcode, "stack_" #TYPE "size()" IF_VERBOSE(, filename, fileline, funcname) );\
    })\
    IF_VERBOSE(\
    fprintf(stderr,LOG_PATTERN "stack_" #TYPE "_size()" LOG_RETURNS "%zu" "\n", filename, fileline, funcname, stk->__size);\
    )\
    return stk->__size;\
  }\


#define DEFINE_STACK_RESIZE(TYPE, TYPE_FORMAT)\
 __attribute__((weak))\
void stack_##TYPE##_resize(stack_##TYPE *stk, size_t newcpct\
        IF_VERBOSE(, const char *filename,)\
        IF_VERBOSE(size_t fileline,)\
        IF_VERBOSE(const char *funcname)\
    ) {\
    IF_VALIDATE(\
    int vcode = stack_##TYPE##_validate(stk);\
    if (vcode) {\
        _stack_##TYPE##_fail(stk, vcode, "stack_"#TYPE"_resize()" IF_VERBOSE(, filename, fileline, funcname) );\
    }\
    )\
    IF_VERBOSE(\
    fprintf(stderr, LOG_PATTERN "stack_"#TYPE"_resize(%zu)\n", filename, fileline, funcname, newcpct);\
    )\
    stk->__cpct = newcpct;\
    IF_CANARY(\
    stk->__arr = (TYPE *)((char *)realloc((char *)stk->__arr - sizeof(CANARY_TYPE), ((newcpct * sizeof(TYPE) - 1) / 8 + 1) * 8 + 2 * sizeof(CANARY_TYPE)) + sizeof(CANARY_TYPE));\
    stk->__data_cnr1 = (CANARY_TYPE *)((char *)stk->__arr - sizeof(CANARY_TYPE));\
    stk->__data_cnr2 = (CANARY_TYPE *)((char *)stk->__arr + ((stk->__cpct * sizeof(TYPE) - 1) / 8 + 1) * 8);\
    CANARY_TYPE *tmp1 = (CANARY_TYPE *)stk->__data_cnr1;\
    *tmp1 = STATIC_CANARY_VAL;\
    CANARY_TYPE *tmp2 = (CANARY_TYPE *)stk->__data_cnr2;\
    *tmp2 = STATIC_CANARY_VAL;\
    )\
    ELSE_CANARY(\
    stk->__arr = reallocarray(stk->__arr, newcpct, sizeof(TYPE));\
    )\
  }

#define DEFINE_STACK_PUSH(TYPE, TYPE_FORMAT)\
__attribute__((weak))\
void stack_##TYPE##_push(stack_##TYPE *stk, TYPE val\
        IF_VERBOSE(,const char *filename,)\
        IF_VERBOSE(size_t fileline,)\
        IF_VERBOSE(const char *funcname)\
    ) {\
    IF_VALIDATE(\
    int vcode = stack_##TYPE##_validate(stk);\
    if (vcode) {         \
        _stack_##TYPE##_fail(stk, vcode, "stack_"#TYPE"_push()" IF_VERBOSE(, filename, fileline, funcname) );\
    }\
    )\
    IF_VERBOSE(\
    fprintf(stderr, LOG_PATTERN "stack_"#TYPE"_push(" TYPE_FORMAT ")\n",\
               filename, fileline, funcname, val);\
    )\
    if (stk->__size == stk->__cpct) {\
        IF_VERBOSE(\
        stack_##TYPE##_resize(stk, stk->__cpct * 2, LOGINFO);\
        )\
        ELSE_VERBOSE(\
        stack_##TYPE##_resize(stk, stk->__cpct * 2);\
        )\
    }\
    stk->__arr[stk->__size++] = val;\
    return /* status code */;\
  }\

#define DEFINE_STACK_POP(TYPE, TYPE_FORMAT)\
__attribute__((weak))\
void stack_##TYPE##_pop(stack_##TYPE *stk\
        IF_VERBOSE(,const char *filename,)\
        IF_VERBOSE(size_t fileline,)\
        IF_VERBOSE(const char *funcname)\
    ) {\
    IF_VALIDATE(\
    int vcode = stack_##TYPE##_validate(stk);\
    if (vcode) {\
        _stack_##TYPE##_fail(stk, vcode, "stack_" #TYPE "_pop()" IF_VERBOSE(, filename, fileline, funcname) );\
    }\
    if (stk->__size == 0) {\
        _stack_##TYPE##_fail(stk, /*pop empty stack*/ 5, "stack_pop()" IF_VERBOSE(, filename, fileline, funcname) );\
        }\
    )\
    IF_VERBOSE(\
    fprintf(stderr, LOG_PATTERN "stack_" #TYPE "_pop()\n",\
        filename, fileline, funcname);\
    )\
    if (stk->__size * 2 < stk->__cpct) {\
        IF_VERBOSE(\
        stack_##TYPE##_resize(stk, stk->__cpct / 2, LOGINFO);\
        )\
        ELSE_VERBOSE(\
        stack_##TYPE##_resize(stk, stk->__cpct / 2);\
        )\
    }\
    stk->__size--;\
  }\

#define DEFINE_STACK_TOP(TYPE, TYPE_FORMAT)\
__attribute__((weak))\
TYPE stack_##TYPE##_top(stack_##TYPE *stk\
        IF_VERBOSE(, const char *filename,)\
        IF_VERBOSE(size_t fileline,)\
        IF_VERBOSE(const char *funcname)\
    ) {\
    IF_VALIDATE(\
    int vcode = stack_##TYPE##_validate(stk);\
    if (vcode) {\
        _stack_##TYPE##_fail(stk, vcode, "stack_" #TYPE "_top()" IF_VERBOSE(, filename, fileline, funcname) );\
    }\
    if (stk->__size == 0) {                                              \
        _stack_##TYPE##_fail(stk, /*accessing empty stack*/ 6, "stack_" #TYPE "_top()" IF_VERBOSE(, filename, fileline, funcname ));\
        }\
    )\
    IF_VERBOSE(\
    fprintf(stderr, LOG_PATTERN "stack_" #TYPE "_top()" LOG_RETURNS TYPE_FORMAT "\n",\
                       filename, fileline, funcname,\
                       stk->__arr[stk->__size - 1]);\
    )\
    return stk->__arr[stk->__size - 1];\
  }

#define DEFINE_STACK(TYPE, TYPE_FORMAT)                                        \
  DEFINE_STACK_STRUCT(TYPE)                                                    \
  DEFINE_STACK_INIT(TYPE)                                                      \
  DEFINE_STACK_DESTROY(TYPE)                                                   \
  DEFINE_STACK_VALIDATE(TYPE)                                                  \
  DEFINE_STACK_FAIL(TYPE)                                                      \
  DEFINE_STACK_DUMP(TYPE, TYPE_FORMAT)                                         \
  DEFINE_STACK_EMPTY(TYPE, TYPE_FORMAT)                                        \
  DEFINE_STACK_SIZE(TYPE, TYPE_FORMAT)                                         \
  DEFINE_STACK_RESIZE(TYPE, TYPE_FORMAT)                                       \
  DEFINE_STACK_PUSH(TYPE, TYPE_FORMAT)                                         \
  DEFINE_STACK_POP(TYPE, TYPE_FORMAT)                                          \
  DEFINE_STACK_TOP(TYPE, TYPE_FORMAT)

#endif // MACROMAGIC_STACK_H
