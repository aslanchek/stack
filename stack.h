#include <stdlib.h> // calloc, realloc, size_t

#ifndef STACK_H
#define STACK_H


#define DEFINE_STACK_STRUCT(TYPE)                  \
typedef struct {                                   \
    size_t __capacity;                             \
    size_t __size;                                 \
    TYPE *__arr;                                   \
} stack_##TYPE;                                    \


#define DEFINE_STACK_INIT(TYPE)                  \
__attribute__((weak))                            \
stack_##TYPE stack_init() {                      \
    return (stack_##TYPE) {                      \
        .__capacity = 2,                         \
        .__size = 0,                             \
        .__arr = calloc(2, sizeof(TYPE)),        \
    };                                           \
}                                                \


#define DEFINE_STACK_DESTROY(TYPE)              \
__attribute__((weak))                           \
void stack_destroy(stack_##TYPE *stk) {         \
    stk->__capacity = 0;                        \
    stk->__size = 0;                            \
    free(stk->__arr);                           \
    stk->__arr = NULL;                          \
}                                               \


#define DEFINE_STACK_EMPTY(TYPE)                \
__attribute__((weak))                           \
int stack_empty(stack_##TYPE *stk) {            \
    return stk->__size == 0;                    \
}                                               \


#define DEFINE_STACK_SIZE(TYPE)                 \
__attribute__((weak))                           \
size_t stack_size(stack_##TYPE *stk) {          \
    return stk->__size;                         \
}                                               \


#define DEFINE_STACK_RESIZE(TYPE)                                 \
__attribute__((weak))                                             \
void stack_resize(stack_##TYPE *stk, size_t newsize) {            \
    stk->__arr = reallocarray(stk->__arr, newsize, sizeof(TYPE)); \
    stk->__capacity = newsize;                                    \
}                                                                 \


#define DEFINE_STACK_PUSH(TYPE)                      \
__attribute__((weak))                                \
void stack_push(stack_##TYPE*stk, TYPE val) {        \
    if (stk->__size == stk->__capacity) {            \
        stack_resize(stk, stk->__capacity * 2);      \
    }                                                \
    stk->__arr[stk->__size++] = val;                 \
};                                                   \


#define DEFINE_STACK_POP(TYPE)             \
__attribute__((weak))                      \
void stack_pop(stack_##TYPE*stk) {         \
    stk->__size--;                         \
}                                          \


#define DEFINE_STACK_TOP(TYPE)             \
__attribute__((weak))                      \
TYPE stack_top(stack_##TYPE*stk) {         \
    return stk->__arr[stk->__size];        \
}                                          \


#define DEFINE_STACK(TYPE)    \
    DEFINE_STACK_STRUCT(TYPE) \
    DEFINE_STACK_INIT(TYPE)   \
    DEFINE_STACK_DESTROY(TYPE)\
    DEFINE_STACK_EMPTY(TYPE)  \
    DEFINE_STACK_SIZE(TYPE)   \
    DEFINE_STACK_RESIZE(TYPE) \
    DEFINE_STACK_PUSH(TYPE)   \
    DEFINE_STACK_POP(TYPE)    \
    DEFINE_STACK_TOP(TYPE)    \

#endif // STACK_H

