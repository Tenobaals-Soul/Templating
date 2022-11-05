#include <open_template.h>
#include <parser.h>
#include <string_dict.h>
#include <list.h>
#include <stack.h>
#include <io_queues.h>
#include <string.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef union any (*filter_chr_type)(char);
typedef union any (*filter_int_type)(long);
typedef union any (*filter_flt_type)(double);
typedef union any (*filter_str_type)(const char*);
typedef union any (*filter_ptr_type)(void*);

struct filter_set {
    int eval_type_of_fchr;
    filter_chr_type filterchr;
    int eval_type_of_fint;
    filter_int_type filterint;
    int eval_type_of_fflt;
    filter_flt_type filterflt;
    int eval_type_of_fstr;
    filter_str_type filterstr;
    int eval_type_of_fptr;
    filter_ptr_type filterptr;
};

typedef struct field_entry {
    eval_type_t eval;
    intptr_t off;
} field_entry_t;

static void __cleanup(list_t exp_list, stack_t pending_string) {
    for (size_t i = 0; i < exp_list->wsize; i++) {
        free_value(get_ptr(exp_list, i));
    }
    list_destroy(exp_list);
    stack_destroy(pending_string);
}

value_t* __compile_statement(input_queue_t instr) {
    reader_t reader = {.queue = instr, .cur = getqueuepos(instr)};
    return parse_expression(&reader, NULL);
}

/**
 * compiles a template
 * 
 * @param instr a input queue defined in "io_queues.h". This contains the template.
 * @param out list of expressions, defined in "parser.h". This list has to be initialized with `list_init(list, sizeof(value_t*));`
 * @param context the operators and functions that can be used in the template
 * @param args user provided positional arguments, can be NULL
 * @param vaargs user provided variables, can be NULL
 * @return int 0 for success and -1 for failiure
 */
int __compile_template(input_queue_t instr, list_t out) {
    stack_t pending_string_data;
    int c;
    bool escaped = false;
    for (;;) {
        switch ((c = popqueue(instr))) {
        case -1:
            if (pending_string_data->bsize > 0) {
                value_string_t* pending_string = malloc(sizeof(value_string_t));
                pending_string->type = VALUE_STRING;
                pending_string->value = stack_disown(pending_string_data);
            }
            else {
                stack_destroy(pending_string_data);
            }
            return 0;
        case '{':
            if (escaped) break;
            value_t* val = __compile_statement(instr);
            while (peekqueue(instr) == ' ' || peekqueue(instr) == '\n') popqueue(instr);
            if (val == NULL || popqueue(instr) != '}') {
                __cleanup(out, pending_string_data);
                return -1;
            }
            break;
        case '\\':
            escaped = !escaped;
            break;
        default:
            push_chr(pending_string_data, c);
        }
    }
}

int __resolve_variable(value_variable_t* val, string_dict_t vaargs) {
    field_entry_t* entry = string_dict_get(vaargs, val->name);
    if (entry == NULL) return -1;
    val->eval = entry->eval;
    val->resolved_off = entry->off;
    return 0;
}

eval_type_t __evaluates_to(value_t* val) {
    switch (*val) {
        case VALUE_CHAR:
            return EVAL_CHAR;
        case VALUE_FLOATING:
            return EVAL_FLOAT;
        case VALUE_INTEGER:
            return EVAL_INT;
        case VALUE_STRING:
            return EVAL_STRING;
        case VALUE_VARIABLE:
            return ((value_variable_t*) val)->eval;
        case VALUE_FILTER:
            return ((value_filter_t*) val)->eval;
        case VALUE_OPERATION:
            return ((value_operation_t*) val)->eval;
    }
    return EVAL_UNSPECIFIED;
}

int __resolve_single_symbol(value_t* val, string_dict_t context, string_dict_t vaargs);

int __resolve_filter(value_filter_t* filter, string_dict_t filter_dict, string_dict_t vaargs) {
    int errc = __resolve_single_symbol(filter->in, filter_dict, vaargs);
    if (errc) return errc;
    eval_type_t eval = __evaluates_to(filter->in);
    if (eval == EVAL_UNSPECIFIED) return -1;
    filter_set_t fset = string_dict_get(filter_dict, filter->name);
    if (fset == NULL) return -1;
    switch (eval) {
    case EVAL_UNSPECIFIED:
        return -1;
    case EVAL_CHAR:
        filter->resolved = fset->filterchr;
        break;
    case EVAL_INT:
        filter->resolved = fset->filterint;
        break;
    case EVAL_FLOAT:
        filter->resolved = fset->filterflt;
        break;
    case EVAL_STRING:
        filter->resolved = fset->filterstr;
        break;
    case EVAL_PTR:
        filter->resolved = fset->filterptr;
        break;
    }
    filter->eval = eval;
    return 0;
}

const char* typecastfilters[] = {
    [EVAL_UNSPECIFIED] = NULL,
    [EVAL_CHAR] = "char",
    [EVAL_INT] = "int",
    [EVAL_FLOAT] = "float",
    [EVAL_PTR] = "obj",
    [EVAL_STRING] = "str"
};

eval_type_t __sync_operaton(value_operation_t* operation, eval_type_t eval_l, eval_type_t eval_r,
                            string_dict_t filter_dict, string_dict_t vaargs) {
    if (eval_l == eval_r) return eval_l;
    eval_type_t common = MAX(eval_l, eval_r);
    if (eval_l != common) {
        value_filter_t* filter = malloc(sizeof(*filter));
        filter->type = VALUE_FILTER;
        filter->in = operation->left;
        filter->name = strdup(typecastfilters[common]);
        filter->eval = common;
        operation->left = (value_t*) filter;
    }
    if (eval_r != common) {
        value_filter_t* filter = malloc(sizeof(*filter));
        filter->type = VALUE_FILTER;
        filter->in = operation->right;
        filter->name = strdup(typecastfilters[common]);
        __resolve_filter(filter, filter_dict, vaargs);
        operation->right = (value_t*) filter;
    }
    return common;
}

int __resolve_operator(value_operation_t* operation, string_dict_t filter_dict, string_dict_t vaargs) {
    int errc;
    errc = __resolve_single_symbol(operation->left, filter_dict, vaargs);
    if (errc) return errc;
    errc = __resolve_single_symbol(operation->right, filter_dict, vaargs);
    if (errc) return errc;
    eval_type_t eval_l = __evaluates_to(operation->left);
    eval_type_t eval_r = __evaluates_to(operation->right);
    if (eval_l == EVAL_UNSPECIFIED || eval_r == EVAL_UNSPECIFIED) return -1;
    eval_type_t eval;
    eval = __sync_operaton(operation, eval_l, eval_r, filter_dict, vaargs);
    operation->eval = eval;
    return 0;
}

int __resolve_single_symbol(value_t* val, string_dict_t context, string_dict_t vaargs) {
    int errc;
    switch (*val) {
    case VALUE_CHAR:
    case VALUE_FLOATING:
    case VALUE_INTEGER:
    case VALUE_STRING:
        break;
    case VALUE_VARIABLE:
        errc = __resolve_variable((value_variable_t*) val, vaargs);
        if (errc) return errc;
        break;
    case VALUE_FILTER:
        errc = __resolve_filter((value_filter_t*) val, context, vaargs);
        if (errc) return errc;
        break;
    case VALUE_OPERATION:
        errc = __resolve_operator((value_operation_t*) val, context, vaargs);
        if (errc) return errc;
        break;
    }
    return -1;
}

int __resolve_symbols(list_t expressions, string_dict_t context, string_dict_t vaargs) {
    for (size_t i = 0; i < expressions->wsize; i++) {
        value_t* val = get_ptr(expressions, i);
        int errc = __resolve_single_symbol(val, context,vaargs);
        if (errc) return errc;
    }
    return 0;
}

int __insert(input_queue_t instr/*, output_queue_t outstr*/, string_dict_t filter_dict, string_dict_t vaargs) {
    list_t expressions;
    list_init(expressions, sizeof(void*));
    int errc;
    errc = __compile_template(instr, expressions);
    if (errc) return errc;
    errc = __resolve_symbols(expressions, filter_dict, vaargs);
    if (errc) return errc;
    // __evaluate(expressions, outstr, args, vaargs);
    return 0;
}