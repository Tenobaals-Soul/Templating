#ifndef INCLUDE_OPEN_TEMPLATE_H
#define INCLUDE_OPEN_TEMPLATE_H
#include <string_dict.h>


typedef enum eval_type {
    EVAL_UNSPECIFIED,
    EVAL_CHAR,
    EVAL_INT,
    EVAL_FLOAT,
    EVAL_PTR,
    EVAL_STRING
} eval_type_t;

union any {
    char c;
    long i;
    char* s;
    double f;
    void* p;
};

typedef struct eval_val {
    int eval_type;
} eval_val_t;

typedef struct filter_set* filter_set_t;

#endif