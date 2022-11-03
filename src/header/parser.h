#ifndef INCLUDE_LVL1PARSER_H
#define INCLUDE_LVL1PARSER_H
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string_dict.h>

typedef struct reader_t {
    char* text;
    size_t cur;
    size_t line;
    size_t loff;
} reader_t;

struct Exception;

struct number {
    enum type {
        NNONE, NINTEGER, NFLOATING
    } type;
    union {
        uintmax_t i;
        double f;
    };
};

typedef struct enviroment_s {
    string_dict_t operators;
} enviroment_t;

#if !defined(TYPES_ONLY)
#if defined(TEST)
char parse_char(reader_t* reader, struct Exception** excptr);
char parse_specific_char(reader_t* reader, struct Exception** excptr, char expect);
char parse_alpha(reader_t* reader, struct Exception** excptr);
char parse_punct(reader_t* reader, struct Exception** excptr);
char parse_digit(reader_t* reader, struct Exception** excptr);
void update_exc(struct Exception** dest, struct Exception* val);
void free_exception(struct Exception* exc);
void skip_whitespace(reader_t* reader);
#ifdef __GNUC__
__attribute__((__format__(__printf__, 4, 5)))
#endif
struct Exception* make_exception(struct Exception* caused_by, size_t parsed_symbols, reader_t r, const char* format, ...);
#endif
void free_exception(struct Exception* exc);
void print_exception(struct Exception* exc);
struct number parse_number(reader_t* reader, struct Exception** excptr);
char parse_character(reader_t* reader, struct Exception** excptr);
char* parse_identifier(reader_t* reader, struct Exception** excptr);
char parse_operator_char(reader_t* reader, struct Exception** excptr);
char* parse_operator(reader_t* reader, struct Exception** excptr, enviroment_t* env);
bool parse_keyword(reader_t* reader, struct Exception** excptr, const char* expect);
char* parse_string(reader_t* reader, struct Exception** excptr);
#endif

typedef enum value_type {
    VALUE_INTEGER,
    VALUE_FLOATING,
    VALUE_STRING,
    VALUE_CHAR,
    VALUE_SCALAR_INITIALIZER,
    VALUE_TUPLE,
    VALUE_OPERATION,
    VALUE_VARIABLE
} value_t;

typedef struct value_integer_s {
    value_t type;
    uintmax_t value;
} value_integer_t;

typedef struct value_floating_s {
    value_t type;
    long double value;
} value_floating_t;

typedef struct value_string_s {
    value_t type;
    char* value;
} value_string_t;

typedef struct value_char_s {
    value_t type;
    char value;
} value_char_t;

typedef struct value_scalar_initializer_s {
    value_t type;
    value_t** value;
    size_t items;
} value_scalar_initializer_t;

typedef struct value_tuple_s {
    value_t type;
    value_t** value;
    size_t items;
} value_tuple_t;

typedef struct value_operation_s {
    value_t type;
    char* operator_name;
    value_t* left;
    value_t* right;
} value_operation_t;

typedef struct value_variable_s {
    value_t type;
    char* name;
} value_variable_t;

value_t* parse_scalar_initializer(reader_t* reader, struct Exception** excptr, enviroment_t* env);
value_t* parse_tuple(reader_t* reader, struct Exception** excptr, enviroment_t* env);
value_t* parse_value(reader_t* reader, struct Exception** excptr, enviroment_t* env);

value_t* parse_expression(reader_t* reader, struct Exception** excptr, enviroment_t* env);

#endif