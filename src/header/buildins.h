#ifndef INCLUDE_BUILDINS_H
#define INCLUDE_BUILDINS_H
#include <stdlib.h>
#include <stdbool.h>
#include <parser.h>
#include <stddef.h>

typedef struct string {
    char* val;
    size_t len;
} string_t;

union any {
    char c;
    long i;
    string_t s;
    double f;
    void* p;
    bool b;
};

typedef enum eval_type {
    EVAL_UNSPECIFIED,
    EVAL_CHAR,
    EVAL_INT,
    EVAL_FLOAT,
    EVAL_PTR,
    EVAL_STRING,
    EVAL_BOOLEAN
} eval_type_t;

typedef struct typed_value {
    eval_type_t type;
    union any val;
} typed_value_t;

struct op_func {
    eval_type_t out_type;
    union any (*add_func)(union any, union any);
};

union any filter_chr_to_string(char c);
union any filter_int_to_string(long i);
union any filter_flt_to_string(double f);
union any filter_str_to_string(const char* s, size_t len);
union any filter_ptr_to_string(void* p);
union any filter_boo_to_string(bool b);

extern struct op_func op_functions[16][7][7];

#endif