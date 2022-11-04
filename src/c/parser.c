#define REQUIRE_LVL0_PARSER
#include <parser.h>
#undef REQUIRE_LVL0_PARSER
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>
#include <errno.h>
#include <stack.h>
#include <stddef.h>
#include <stdbool.h>
#include <memory.h>
#include <limits.h>

typedef struct Exception {
    char* val;
    struct Exception* caused_by;
    size_t parsed_symbols;
    char* text_pos;
} Exception;

static void print_single_exception(Exception* exc) {
    char obuf[16];
    size_t len;
    for (len = 0; exc->text_pos[len] && exc->text_pos[len] != '\n' && len < 16; len++);
    if (len > 15) {
        memcpy(obuf, exc->text_pos, 12);
        strcpy(obuf + 12, "...");
    }
    else {
        memcpy(obuf, exc->text_pos, len);
        obuf[len] = 0;
    }
    printf("%s. At: %s", exc->val, obuf);
}

void print_exception(Exception* exc) {
    print_single_exception(exc);
    printf("\n");
    while (exc->caused_by) {
        exc = exc->caused_by;
        printf("    ");
        print_single_exception(exc);
        printf("\n");
    }
}

void update_exc(Exception** dest, Exception* val) {
    if (dest != NULL) {
        if (*dest == NULL) {
            *dest = val;
            return;
        }
        if (val == NULL) {
            free_exception(*dest);
            *dest = NULL;
            return;
        }
        if ((*dest)->parsed_symbols <= val->parsed_symbols) {
            free_exception(*dest);
            *dest = val;
            return;
        }
        free_exception(val);
    }
    else if (val) free_exception(val);
}

void free_exception(Exception* exc) {
    Exception* next;
    while (exc) {
        next = exc->caused_by;
        free(exc->val);
        free(exc);
        exc = next;
    }
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 4, 5)))
#endif
Exception* make_exception(Exception* caused_by, size_t parsed_symbols, reader_t r, const char* format, ...) {
    va_list l;
    va_start(l, format);
    int len = vsnprintf(NULL, 0, format, l);
    va_end(l);
    char* buffer = malloc(len + 1);
    va_start(l, format);
    vsprintf(buffer, format, l);
    va_end(l);
    Exception* exc = malloc(sizeof(Exception));
    exc->caused_by = caused_by;
    exc->parsed_symbols = parsed_symbols;
    seekqueue(r.queue, r.cur);
    peekqueuemany(r.queue, 16, &exc->text_pos);
    exc->val = buffer;
    return exc;
}

char parse_char(reader_t* reader, Exception** excptr) {
    seekqueue(reader->queue, reader->cur);
    int out = popqueue(reader->queue);
    if (out == -1) update_exc(excptr, make_exception(NULL, 0, *reader, "EOF"));
    return out;
}

char parse_specific_char(reader_t* reader, Exception** excptr, char expect) {
    reader_t r = *reader;
    Exception* exc = NULL;
    char out = parse_char(&r, &exc);
    if (out == 0) {
        exc = make_exception(exc, 0, r, "no digit was found");
        update_exc(excptr, exc);
        return 0;
    }
    if (out != expect) {
        exc = make_exception(NULL, 0, r, "expected '%c', not '%c'", expect, out);
        update_exc(excptr, exc);
        return 0;
    }
    else {
        *reader = r;
        update_exc(excptr, NULL);
        return out;
    }
}

char parse_alpha(reader_t* reader, Exception** excptr) {
    reader_t r = *reader;
    Exception* exc = NULL;
    char out = parse_char(&r, &exc);
    if (out == 0) {
        update_exc(excptr, make_exception(exc, 0, r, "no alphabetic character was found"));
        return 0;
    }
    if (!isalpha(out)) {
        update_exc(excptr, make_exception(NULL, 0, r, "'%c' is not an alphabetical character", out));
        return 0;
    }
    else {
        *reader = r;
        update_exc(excptr, NULL);
        return out;
    }
}

char parse_punct(reader_t* reader, Exception** excptr) {
    reader_t r = *reader;
    Exception* exc = NULL;
    char out = parse_char(&r, &exc);
    if (out == 0) {
        update_exc(excptr, make_exception(exc, 0, r, "no punctuation character was found"));
        return 0;
    }
    if (!ispunct(out)) {
        update_exc(excptr, make_exception(NULL, 0, r, "'%c' is not a punctuation character", out));
        return 0;
    }
    else {
        *reader = r;
        update_exc(excptr, NULL);
        return out;
    }
}

char parse_digit(reader_t* reader, Exception** excptr) {
    reader_t r = *reader;
    Exception* exc = NULL;
    char out = parse_char(&r, &exc);
    if (out == 0) {
        update_exc(excptr, make_exception(exc, 0, r, "no digit was found"));
        return 0;
    }
    if (!isdigit(out)) {
        update_exc(excptr, make_exception(NULL, 0, r, "'%c' is not a digit", out));
        return 0;
    }
    else {
        *reader = r;
        update_exc(excptr, NULL);
        return out;
    }
}

struct number parse_number(reader_t* reader, struct Exception** excptr) {
    seekqueue(reader->queue, reader->cur);
    char* startptr;
    peekqueuemany(reader->queue, 64, &startptr);
    char* errptri;
    char* errptrf;
    uintmax_t iout = strtoumax(startptr, &errptri, 0);
    int ierr = errno;
    double fout = strtod(startptr, &errptrf);
    int ferr = errno;
    char* errptr, err;
    struct number to_ret;
    if (errptri >= errptrf) {
        errptr = errptri;
        err = ierr;
        to_ret.type = NINTEGER;
        to_ret.i = iout;
    }
    else {
        errptr = errptrf;
        err = ferr;
        to_ret.type = NFLOATING;
        to_ret.f = fout;
    }
    if (errptr == startptr) {
        update_exc(excptr, make_exception(NULL, 0, *reader, "not a number"));
        free(startptr);
        return (struct number) {0};
    }
    if (err == ERANGE) {
        update_exc(excptr, make_exception(NULL, 1, *reader, "number too big"));
        free(startptr);
        return (struct number) {0};
    }
    update_exc(excptr, NULL);
    reader->cur += errptr - startptr;
    free(startptr);
    return to_ret;
}

char* parse_identifier(reader_t* reader, Exception** excptr) {
    stack_t stack;
    init_stack(stack);
    char c;
    Exception* exc = NULL;
    if ((c = parse_alpha(reader, &exc)) ||
        (c = parse_specific_char(reader, &exc, '_')));
    else {
        update_exc(excptr, make_exception(exc, 0, *reader, "not an identifier"));
        return NULL;
    }
    push_chr(stack, c);
    for (;;) {
        if ((c = parse_alpha(reader, NULL)) ||
            (c = parse_specific_char(reader, NULL, '_')) ||
            (c = parse_digit(reader, NULL))) {
            push_chr(stack, c);
        }
        else break;
    }
    update_exc(excptr, NULL);
    return stack_disown(stack);
}

bool parse_keyword(reader_t* reader, Exception** excptr, const char* expect) {
    reader_t r = *reader;
    for (size_t i = 0; expect[i]; i++) {
        Exception* exc = NULL;
        if (parse_specific_char(&r, &exc, expect[i]) == 0) {
            update_exc(excptr, make_exception(exc, 0, r, "expected \"%s\"", expect));
            return false;
        }
    }
    *reader = r;
    update_exc(excptr, NULL);
    return true;
}

char* parse_operator(reader_t* reader, Exception** excptr, context_t env) {
    Exception* exc = NULL;
    for (dict_iter_t i = get_dict_iter(env->operators); i.hasnext; dict_iter_advance(&i)) {
        if (parse_keyword(reader, &exc, i.key)) {
            if (exc) free_exception(exc);
            return strdup(i.key);
        }
    }
    update_exc(excptr, make_exception(exc, 0, *reader, "no operator found"));
    return NULL;
}

char parse_character(reader_t* reader, Exception** excptr) {
    reader_t r = *reader;
    Exception* exc = NULL;
    char c;
    if (parse_specific_char(&r, &exc, '\'') == 0) {
        update_exc(excptr, make_exception(exc, 0, r, "expected a character"));
        return 0;
    }
    if ((c = parse_char(&r, &exc)) == 0) { // fix later to support excape sequences
        update_exc(excptr, make_exception(exc, 1, r, "expected a character"));
        return 0;
    }
    if (parse_specific_char(&r, &exc, '\'') == 0) {
        update_exc(excptr, make_exception(exc, 1, r, "expected a closing '"));
        return 0;
    }
    update_exc(excptr, NULL);
    *reader = r;
    return c;
}

char* parse_string(reader_t* reader, Exception** excptr, size_t* size) {
    reader_t r = *reader;
    Exception* exc = NULL;
    if (parse_specific_char(&r, &exc, '\"') == 0) {
        update_exc(excptr, make_exception(exc, 0, r, "expected a string"));
        return NULL;
    }
    stack_t stack;
    init_stack(stack);
    for (;;) {
        char c;
        if ((c = parse_char(&r, &exc)) == 0 || c == '\n') {
            update_exc(excptr, make_exception(exc, 1, r, "no closing \" was found"));
            stack_destroy(stack);
            return NULL;
        }
        else if (c == '\"') break;
        push_chr(stack, c);
    }
    update_exc(excptr, NULL);
    *reader = r;
    if (size) *size = stack->bsize;
    return stack_disown(stack);
}

void skip_whitespace(reader_t* reader);

value_t* parse_number_value(reader_t* reader, Exception** excptr) {
    Exception* exc = NULL;
    struct number out_val = parse_number(reader, &exc);
    if (out_val.type == NNONE) {
        update_exc(excptr, exc);
        return NULL;
    }
    if (out_val.type == NINTEGER) {
        value_integer_t* out_wrapper = malloc(sizeof(value_integer_t));
        update_exc(excptr, NULL);
        out_wrapper->type = VALUE_INTEGER;
        out_wrapper->value = out_val.i;
        return &out_wrapper->type;
    }
    else {
        value_floating_t* out_wrapper = malloc(sizeof(value_floating_t));
        update_exc(excptr, NULL);
        out_wrapper->type = VALUE_FLOATING;
        out_wrapper->value = out_val.f;
        return &out_wrapper->type;
    }
}

value_t* parse_character_value(reader_t* reader, Exception** excptr) {
    Exception* exc = NULL;
    char out_val = parse_character(reader, &exc);
    value_char_t* out_wrapper = calloc(1, sizeof(*out_wrapper));
    if (exc) {
        update_exc(excptr, exc);
        return NULL;
    }
    else {
        update_exc(excptr, NULL);
        out_wrapper->type = VALUE_CHAR;
        out_wrapper->value = out_val;
        return &out_wrapper->type;
    }
}

value_t* parse_string_value(reader_t* reader, Exception** excptr) {
    Exception* exc = NULL;
    size_t size;
    char* out_val = parse_string(reader, &exc, &size);
    value_string_t* out_wrapper = calloc(1, sizeof(*out_wrapper));
    if (exc) {
        update_exc(excptr, exc);
        return NULL;
    }
    else {
        update_exc(excptr, NULL);
        out_wrapper->type = VALUE_STRING;
        out_wrapper->value = out_val;
        out_wrapper->actual_length = size;
        return &out_wrapper->type;
    }
}

void free_value(value_t* val) {
    if (val == NULL) return;
    switch (*val) {
    case VALUE_CHAR:
    case VALUE_FLOATING:
    case VALUE_INTEGER:
        break;
    case VALUE_STRING:
        free(((value_string_t*) val)->value);
        break;
    case VALUE_OPERATION:
        free_value(((value_operation_t*) val)->left);
        free_value(((value_operation_t*) val)->right);
        free(((value_operation_t*) val)->operator_name);
        break;
    case VALUE_VARIABLE:
        free(((value_variable_t*) val)->name);
    case VALUE_FILTER:
        free(((value_filter_t*) val)->name);
    }
    free(val);
}

static void free_value_stack(stack_t stack) {
    value_t* val;
    while ((val = pop_ptr(stack))) {
        free_value(val);
    }
    stack_destroy(stack);
}

value_t* parse_variable(reader_t* reader, Exception** excptr) {
    skip_whitespace(reader);
    char* name = parse_identifier(reader, excptr);
    if (name == NULL)
        return NULL;
    value_variable_t* v = malloc(sizeof(*v));
    v->type = VALUE_VARIABLE;
    v->name = name;
    return (value_t*) v;
}

value_t* parse_filter(reader_t* reader, Exception** excptr) {
    reader_t r = *reader;
    Exception* exc;
    if (!parse_specific_char(&r, &exc, '|')) {
        update_exc(excptr, make_exception(exc, 0, r, "expected a pipe character | for filter"));
        return NULL;
    }
    char* name = parse_identifier(&r, &exc);
    if (name == NULL) {
        update_exc(excptr, make_exception(exc, 0, r, "expected filter name"));
        return NULL;
    }
    value_filter_t* out = malloc(sizeof(value_filter_t));
    out->type = VALUE_FILTER;
    out->name = name;
    return out;
}

value_t* parse_value(reader_t* reader, Exception** excptr, context_t env) {
    Exception* temp_exc = NULL;
    Exception* exc = NULL;
    value_t* out;
    skip_whitespace(reader);
    if ((out = parse_filter(reader, &exc))) {
        if (temp_exc) free_exception(temp_exc);
        return out;
    }
    else update_exc(&temp_exc, exc);
    if ((out = parse_character_value(reader, &exc))) {
        if (temp_exc) free_exception(temp_exc);
        return out;
    }
    else update_exc(&temp_exc, exc);
    exc = NULL;
    if ((out = parse_number_value(reader, &exc))) {
        if (temp_exc) free_exception(temp_exc);
        return out;
    }
    else update_exc(&temp_exc, exc);
    exc = NULL;
    if ((out = parse_string_value(reader, &exc))) {
        if (temp_exc) free_exception(temp_exc);
        return out;
    }
    else update_exc(&temp_exc, exc);
    exc = NULL;
    if ((out = parse_variable(reader, &exc))) {
        if (temp_exc) free_exception(temp_exc);
        return out;
    }
    else update_exc(&temp_exc, exc);
    if(excptr) *excptr = temp_exc;
    else free_exception(temp_exc);
    return NULL;
}

void skip_whitespace(reader_t* reader) {
    reader_t r = *reader;
    for (;;) {
        switch (parse_char(&r, NULL)) {
        case ' ':
        case '\n':
            break;
        case 0:
            *reader = r;
            return;
        default:
            r.cur--;
            *reader = r;
            return;
        }
    }
}

enum mode {
    SEARCH_VALUE, SEARCH_OPERATOR
};

struct expression_parsing_locals {
    reader_t r;
    Exception* exc;
    stack_t vastack;
    stack_t opstack;
    enum mode mode;
    size_t values_parsed;
};

bool parse_expression_value(context_t env, struct expression_parsing_locals* l) {
    value_t* val = parse_value(&l->r, &l->exc, env);
    if (val) {
        push_ptr(l->vastack, val);
        l->values_parsed++;
    }
    else {
        free_value_stack(l->vastack);
        while (peek_ptr(l->opstack)) {
            free(pop_ptr(l->opstack));
        }
        l->exc = make_exception(l->exc, l->values_parsed, l->r,
            "could not parse expression because a %s was expected",
            l->mode == SEARCH_VALUE ? "value" : "operator");
        return false;
    }
    l->mode = SEARCH_OPERATOR;
    return true;
}

int priority(const char* op) {
    if (op == NULL) return INT_MIN;
    if (strcmp(op, "&&") == 0) return 0;
    if (strcmp(op, "||") == 0) return 0;

    if (strcmp(op, "==") == 0) return 0;
    if (strcmp(op, "!=") == 0) return 0;

    if (strcmp(op, "<=") == 0) return 1;
    if (strcmp(op, ">=") == 0) return 1;
    if (strcmp(op, "<") == 0) return 2;
    if (strcmp(op, ">") == 0) return 2;

    if (strcmp(op, "+") == 0) return 3;
    if (strcmp(op, "-") == 0) return 3;


    if (strcmp(op, "*") == 0) return 4;
    if (strcmp(op, "/") == 0) return 4;

    if (strcmp(op, "|") == 0) return 5;

    if (strcmp(op, "&") == 0) return 6;

    if (strcmp(op, "<<") == 0) return 7;
    if (strcmp(op, ">>") == 0) return 7;
    else return -1;
}

void pushback(stack_t vastack, stack_t opstack, char* op) {
    while (peek_ptr(opstack) && priority(op) <= priority((((value_operation_t*) peek_ptr(opstack))->operator_name))) {
        value_operation_t* opw = pop_ptr(opstack);
        opw->right = pop_ptr(vastack);
        opw->left = pop_ptr(vastack);
        push_ptr(vastack, opw);
    }
}

bool parse_expression_operator(context_t env, struct expression_parsing_locals* l) {
    char* op = parse_operator(&l->r, &l->exc, env);
    if (op != NULL) {
        value_operation_t* opwrapper = malloc(sizeof(value_operation_t));
        opwrapper->type = VALUE_OPERATION;
        opwrapper->operator_name = op;
        pushback(l->vastack, l->opstack, op);
        push_ptr(l->opstack, opwrapper);
        l->values_parsed++;
        l->mode = SEARCH_VALUE;
        return true;
    }
    return false;
}

value_t* parse_expression(reader_t* reader, Exception** excptr, context_t env) {
    struct expression_parsing_locals l;
    l.r = *reader;
    l.exc = NULL;
    init_stack(l.vastack);
    init_stack(l.opstack);
    l.mode = SEARCH_VALUE;
    l.values_parsed = 0;
    for (;;) {
        skip_whitespace(&l.r);
        if (l.mode == SEARCH_VALUE) {
            if (!parse_expression_value(env, &l)) {
                update_exc(excptr, l.exc);
                return NULL;
            }
        }
        else {
            if (!parse_expression_operator(env, &l)) {
                pushback(l.vastack, l.opstack, NULL);
                value_t* val = pop_ptr(l.vastack);
                stack_destroy(l.vastack);
                stack_destroy(l.opstack);
                *reader = l.r;
                return val;
            }
        }
    }
}
