#ifndef STACK_H
#define STACK_H

#include <stdlib.h> // calloc, realloc, size_t

#ifdef USEDUMP
#include <stdio.h>
#endif


#define LOG_PATTERN  "[stack log] %s:%zu in (%s): "
#define LOG_RETURNS  " -> "

#define DEFINE_STACK_STRUCT(TYPE)                  \
typedef struct {                                   \
    size_t __capacity;                             \
    size_t __size;                                 \
    TYPE *__arr;                                   \
} stack_##TYPE;                                    \


#define DEFINE_STACK_INIT(TYPE)                  \
__attribute__((weak))                            \
stack_##TYPE stack_##TYPE##_init() {             \
    return (stack_##TYPE) {                      \
        .__capacity = 2,                         \
        .__size = 0,                             \
        .__arr = calloc(2, sizeof(TYPE)),        \
    };                                           \
}                                                \


#define DEFINE_STACK_DESTROY(TYPE)               \
__attribute__((weak))                            \
void stack_##TYPE##_destroy(stack_##TYPE *stk) { \
    stk->__capacity = 0;                         \
    stk->__size = 0;                             \
    free(stk->__arr);                            \
    stk->__arr = NULL;                           \
}                                                \


#ifdef USEDUMP
#define IF_USEDUMP(...) __VA_ARGS__
#else
#define IF_USEDUMP(...)
#endif


#define DEFINE_STACK_DUMP(TYPE, TYPE_FORMAT)                                 \
__attribute__((weak))                                                        \
void _stack_##TYPE##_dump(stack_##TYPE *stk,                                 \
            const  char *objname,                                            \
            const char *filename,                                            \
            const size_t line,                                               \
            const char *funcname) {                                          \
    IF_USEDUMP(                                                              \
    fprintf(stderr, LOG_PATTERN "stack_dump()\n", filename, line, funcname); \
    fprintf(stderr, "struct<"#TYPE"> [%p] \"%s\" {\n", stk, objname);        \
    fprintf(stderr,                                                          \
        "  capacity = %zu\n"                                                 \
        "  size     = %zu\n"                                                 \
        "  data [%p] {\n",                                                   \
          stk->__capacity, stk->__size, stk->__arr);                         \
    for (size_t i = 0; i < stk->__size; i++) {                               \
        fprintf(stderr, "    *[%zu] = "TYPE_FORMAT"\n", i, stk->__arr[i]);   \
    }                                                                        \
    for (size_t i = stk->__size; i < stk->__capacity; i++) {                 \
        fprintf(stderr, "     [%zu] = "TYPE_FORMAT"\n", i, stk->__arr[i]);   \
    }                                                                        \
    fprintf(stderr, "  }\n");                                                \
    fprintf(stderr, "}\n");                                                  \
    )                                                                        \
}                                                                            \


#define STACK_DUMP(stk, TYPE) _stack_##TYPE##_dump(stk, (#stk), __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__)


#define DEFINE_STACK_EMPTY(TYPE)                \
__attribute__((weak))                           \
int stack_##TYPE##_empty(stack_##TYPE *stk) {   \
    return stk->__size == 0;                    \
}                                               \


#define DEFINE_STACK_SIZE(TYPE)                 \
__attribute__((weak))                           \
size_t stack_##TYPE##_size(stack_##TYPE *stk) { \
    return stk->__size;                         \
}                                               \


#define DEFINE_STACK_RESIZE(TYPE)                                 \
__attribute__((weak))                                             \
void stack_##TYPE##_resize(stack_##TYPE *stk, size_t newsize) {   \
    stk->__arr = reallocarray(stk->__arr, newsize, sizeof(TYPE)); \
    stk->__capacity = newsize;                                    \
}                                                                 \


#define DEFINE_STACK_PUSH(TYPE)                          \
__attribute__((weak))                                    \
void stack_##TYPE##_push(stack_##TYPE*stk, TYPE val) {   \
    if (stk->__size == stk->__capacity) {                \
        stack_##TYPE##_resize(stk, stk->__capacity * 2); \
    }                                                    \
    stk->__arr[stk->__size++] = val;                     \
};                                                       \


#define DEFINE_STACK_POP(TYPE)              \
__attribute__((weak))                       \
void stack_##TYPE##_pop(stack_##TYPE*stk) { \
    stk->__size--;                          \
}                                           \


#define DEFINE_STACK_TOP(TYPE)               \
__attribute__((weak))                        \
TYPE stack_##TYPE##_top(stack_##TYPE*stk) {  \
    return stk->__arr[stk->__size];          \
}                                            \


#define DEFINE_STACK(TYPE, TYPE_FORMAT)    \
    DEFINE_STACK_STRUCT(TYPE)              \
    DEFINE_STACK_INIT(TYPE)                \
    DEFINE_STACK_DESTROY(TYPE)             \
    DEFINE_STACK_DUMP(TYPE, TYPE_FORMAT)   \
    DEFINE_STACK_EMPTY(TYPE)               \
    DEFINE_STACK_SIZE(TYPE)                \
    DEFINE_STACK_RESIZE(TYPE)              \
    DEFINE_STACK_PUSH(TYPE)                \
    DEFINE_STACK_POP(TYPE)                 \
    DEFINE_STACK_TOP(TYPE)                 \

#endif // STACK_H

