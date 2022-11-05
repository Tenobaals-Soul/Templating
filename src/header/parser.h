#ifndef INCLUDE_LVL1PARSER_H
#define INCLUDE_LVL1PARSER_H
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string_dict.h>
#include <io_queues.h>

typedef struct {
    input_queue_t queue;
    size_t cur;
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

enum operator {
    AND, OR,
    EQUALS, NEQUALS,
    LEQUALS, GEQUALS,
    LESS, GREATER,
    ADD, SUBTRACT,
    MULTIPLY, DIVIDE,
    BINOR, BINAND,
    LSHIFT, RSHIFT
};

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
enum value_type* parse_value(reader_t* reader, struct Exception** excptr);
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
int parse_operator(reader_t* reader, struct Exception** excptr);
bool parse_keyword(reader_t* reader, struct Exception** excptr, const char* expect);
char* parse_string(reader_t* reader, struct Exception** excptr, size_t* size);
#endif

typedef enum value_type {
    VALUE_INTEGER,
    VALUE_FLOATING,
    VALUE_STRING,
    VALUE_CHAR,
    VALUE_OPERATION,
    VALUE_VARIABLE,
    VALUE_FILTER
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
    size_t actual_length;
} value_string_t;

typedef struct value_char_s {
    value_t type;
    char value;
} value_char_t;

typedef struct value_operation_s {
    value_t type;
    int eval;
    int operatorno;
    value_t* left;
    value_t* right;
} value_operation_t;

typedef struct value_variable_s {
    value_t type;
    int eval;
    char* name;
    size_t resolved_off;
} value_variable_t;

typedef struct value_filter_s {
    value_t type;
    value_t* in;
    int eval;
    char* name;
    void* resolved;
} value_filter_t;

void free_value(value_t* val);

value_t* parse_expression(reader_t* reader, struct Exception** excptr);

#endif