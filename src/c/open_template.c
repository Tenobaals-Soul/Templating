#include <open_template.h>
#include <parser.h>
#include <string_dict.h>
#include <list.h>
#include <stack.h>
#include <io_queues.h>
#include <string.h>
#include <assert.h>
#include <buildins.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

struct filter_set {
    int eval_type_of_fchr;
    union any (*filterchr)(char c);
    int eval_type_of_fint;
    union any (*filterint)(long i);
    int eval_type_of_fflt;
    union any (*filterflt)(double f);
    int eval_type_of_fstr;
    union any (*filterstr)(const char* val, size_t len);
    int eval_type_of_fptr;
    union any (*filterptr)(void* i);
    int eval_type_of_fboo;
    union any (*filterboo)(bool i);
};

struct filter_set str_filter_set = {
    EVAL_STRING,
    filter_chr_to_string,
    EVAL_STRING,
    filter_int_to_string,
    EVAL_STRING,
    filter_flt_to_string,
    EVAL_STRING,
    filter_str_to_string,
    EVAL_STRING,
    filter_ptr_to_string,
    EVAL_STRING,
    filter_boo_to_string
};

typedef struct field_entry {
    eval_type_t eval;
    intptr_t off;
} field_entry_t;

static void cleanup(list_t exp_list, stack_t pending_string) {
    for (size_t i = 0; i < exp_list->wsize; i++) {
        free_value(get_ptr(exp_list, i));
    }
    list_destroy(exp_list);
    stack_destroy(pending_string);
}

value_t* compile_statement(input_queue_t instr) {
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
int compile_template(input_queue_t instr, list_t out) {
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
            value_t* val = compile_statement(instr);
            while (peekqueue(instr) == ' ' || peekqueue(instr) == '\n') popqueue(instr);
            if (val == NULL || popqueue(instr) != '}') {
                cleanup(out, pending_string_data);
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

int resolve_variable(value_variable_t* val, string_dict_t vaargs) {
    field_entry_t* entry = string_dict_get(vaargs, val->name);
    if (entry == NULL) return -1;
    val->eval = entry->eval;
    val->resolved_off = entry->off;
    return 0;
}

__attribute__((__const__))
eval_type_t evaluates_to(value_t* val) {
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

int resolve_single_symbol(value_t* val, string_dict_t context, string_dict_t vaargs);

int resolve_filter(value_filter_t* filter, string_dict_t filter_dict, string_dict_t vaargs) {
    int errc = resolve_single_symbol(filter->in, filter_dict, vaargs);
    if (errc) return errc;
    eval_type_t eval = evaluates_to(filter->in);
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
    case EVAL_BOOLEAN:
        filter->resolved = fset->filterboo;
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

eval_type_t find_second_operator(eval_type_t eval_r, eval_type_t op[5]) {
    return op[eval_r];
}

eval_type_t find_operator(value_operation_t* operation, eval_type_t eval_l, eval_type_t eval_r) {
    return op_functions[operation->operatorno][eval_l][eval_r].out_type;
}

int resolve_operator(value_operation_t* operation, string_dict_t filter_dict, string_dict_t vaargs) {
    int errc;
    errc = resolve_single_symbol(operation->left, filter_dict, vaargs);
    if (errc) return errc;
    errc = resolve_single_symbol(operation->right, filter_dict, vaargs);
    if (errc) return errc;
    eval_type_t eval_l = evaluates_to(operation->left);
    eval_type_t eval_r = evaluates_to(operation->right);
    if (eval_l == EVAL_UNSPECIFIED || eval_r == EVAL_UNSPECIFIED) return -1;
    eval_type_t eval;
    eval = find_operator(operation, eval_l, eval_r);
    if (eval == EVAL_UNSPECIFIED) return -1;
    operation->eval = eval;
    return 0;
}

int resolve_single_symbol(value_t* val, string_dict_t context, string_dict_t vaargs) {
    int errc;
    switch (*val) {
    case VALUE_CHAR:
    case VALUE_FLOATING:
    case VALUE_INTEGER:
    case VALUE_STRING:
        break;
    case VALUE_VARIABLE:
        errc = resolve_variable((value_variable_t*) val, vaargs);
        if (errc) return errc;
        break;
    case VALUE_FILTER:
        errc = resolve_filter((value_filter_t*) val, context, vaargs);
        if (errc) return errc;
        break;
    case VALUE_OPERATION:
        errc = resolve_operator((value_operation_t*) val, context, vaargs);
        if (errc) return errc;
        break;
    }
    return -1;
}

int resolve_symbols(list_t expressions, string_dict_t context, string_dict_t vaargs) {
    for (size_t i = 0; i < expressions->wsize; i++) {
        value_t* val = get_ptr(expressions, i);
        int errc = resolve_single_symbol(val, context,vaargs);
        if (errc) return errc;
    }
    return 0;
}

static union any evaluate_single_expression(value_t* expression, void* data);

static inline union any apply_filter(void* filter, value_t* inner, void* data) {
    union any in = evaluate_single_expression(inner, data);
    union any out;
    switch (evaluates_to(inner)) {
    case EVAL_UNSPECIFIED:
        out = (union any) {.s = { strdup(""), 0 }};
        break;
    case EVAL_CHAR:
        out = ((filter_chr_type) filter)(in.c);
        break;
    case EVAL_INT:
        out = ((filter_int_type) filter)(in.i);
        break;
    case EVAL_FLOAT:
        out = ((filter_flt_type) filter)(in.f);
        break;
    case EVAL_STRING:
        string_t s = in.s;
        out = ((filter_str_type) filter)(s.val, s.len);
        break;
    case EVAL_PTR:
        out = ((filter_ptr_type) filter)(in.p);
        break;
    case EVAL_BOOLEAN:
        out = ((filter_boo_type) filter)(in.b);
        break;
    }
    if (evaluates_to(inner) == EVAL_STRING) { free(in.s.val); }
    return out;
}

static union any evaluate_operation(value_operation_t* op, void* data) {
    union any left = evaluate_single_expression(op->left, data);
    union any right = evaluate_single_expression(op->right, data);
    union any result = op_functions[op->operatorno][evaluates_to(op->left)][evaluates_to(op->right)].add_func(left, right);
    if (evaluates_to(op->left) == EVAL_STRING) { free(left.s.val); }
    if (evaluates_to(op->right) == EVAL_STRING) { free(right.s.val); }
    return result;
}

static union any evaluate_variable(value_variable_t* off, void* data) {
    char* pos = ((char*) data) + off->resolved_off;
    switch (evaluates_to((value_t*) off)) {
    case EVAL_UNSPECIFIED:
        break;
    case EVAL_CHAR:
        return (union any) {.b = *((char*) pos)};
    case EVAL_INT:
        return (union any) {.i = *((long*) pos)};
    case EVAL_FLOAT:
        return (union any) {.f = *((double*) pos)};
    case EVAL_PTR:
        return (union any) {.p = *((void**) pos)};
    case EVAL_STRING:
        return (union any) {.s = { *((char**) pos), *((size_t*) (pos + sizeof(char*)))}};
    case EVAL_BOOLEAN:
        return (union any) {.b = *((bool*) pos)};
    }
    return (union any) {.s = {strdup(""), 0}};
}

static union any evaluate_single_expression(value_t* expression, void* data) {
    switch (*expression) {
    case VALUE_OPERATION:
        return evaluate_operation((value_operation_t*) expression, data);
    case VALUE_CHAR:
        return (union any) {.c = ((value_char_t*) expression)->value};
    case VALUE_FLOATING:
        return (union any) {.f = ((value_floating_t*) expression)->value};
    case VALUE_INTEGER:
        return (union any) {.i = ((value_integer_t*) expression)->value};
    case VALUE_STRING:
        return (union any) {.s = {memcpy(
            malloc(((value_string_t*) expression)->actual_length + 1),
            ((value_string_t*) expression)->value,
            ((value_string_t*) expression)->actual_length + 1
        )}};
    case VALUE_VARIABLE:
        return evaluate_variable((value_variable_t*) expression, data);
    case VALUE_FILTER:
        return apply_filter(
            ((value_filter_t*) expression)->resolved,
            ((value_filter_t*) expression)->in,
            data
        );
    }
    return (union any) {.s = {strdup(""), 0}};
}

void* create_data_dynamically(string_dict_t vaargs, string_dict_t vaargsoffet) {
    string_dict_init(vaargsoffet);
    stack_t data;
    init_stack(data);
    for (dict_iter_t i = get_dict_iter(vaargs); i.hasnext; dict_iter_advance(&i)) {
        string_dict_put(vaargsoffet, i.key, (void*) (intptr_t) data->bsize);
        switch (((typed_value_t*) i.val)->type) {
        case EVAL_CHAR:
            push_chr(data, ((typed_value_t*) i.val)->val.c);
            break;
        case EVAL_INT:
            push_lng(data, ((typed_value_t*) i.val)->val.i);
            break;
        case EVAL_FLOAT:
#if SIZEOF_DOUBLE == SIZEOF_LONG
            push_lng(data, ((typed_value_t*) i.val)->val.f);
#elif SIZEOF_DOUBLE == SIZEOF_LONG_LONG
            push_llg(data, ((typed_value_t*) i.val)->val.f);
#endif
            break;
        case EVAL_UNSPECIFIED:
        case EVAL_STRING:
            push_ptr(data, ((typed_value_t*) i.val)->val.s.val);
            push_lng(data, ((typed_value_t*) i.val)->val.s.len);
            break;
        case EVAL_PTR:
            push_ptr(data, ((typed_value_t*) i.val)->val.p);
            break;
        case EVAL_BOOLEAN:
            push_chr(data, ((typed_value_t*) i.val)->val.b);
            break;
        }
    }
    return stack_disown(data);
}

string_t tostrinternal(union any val, eval_type_t type) {
    switch (type) {
    case EVAL_UNSPECIFIED:
        break;
    case EVAL_CHAR:
        return filter_chr_to_string(val.c).s;
    case EVAL_INT:
        return filter_int_to_string(val.i).s;
    case EVAL_FLOAT:
        return filter_flt_to_string(val.f).s;
    case EVAL_STRING:
        return val.s;
    case EVAL_PTR:
        return filter_ptr_to_string(val.p).s;
    case EVAL_BOOLEAN:
        return filter_boo_to_string(val.b).s;
    }
    return (string_t) {strdup(""), 0};
}

void evaluate(list_t expressions, output_queue_t outstr, void* data) {
    for (size_t i = 0; i < expressions->wsize; i++) {
        value_t* exp = get_ptr(expressions, i);
        union any val = evaluate_single_expression(exp, data);
        string_t str = tostrinternal(val, evaluates_to(exp));
        pushqueuemany(outstr, str.val, str.len);
    }
}

int insert(input_queue_t instr, output_queue_t outstr, string_dict_t filter_dict, string_dict_t vaargs) {
    list_t expressions;
    list_init(expressions, sizeof(void*));
    int errc;
    errc = compile_template(instr, expressions);
    if (errc) return errc;
    string_dict_t vaargsoffset;
    void* data = create_data_dynamically(vaargs, vaargsoffset);
    errc = resolve_symbols(expressions, filter_dict, vaargsoffset);
    string_dict_destroy(vaargsoffset);
    if (errc) return errc;
    evaluate(expressions, outstr, data);
    free(data);
    return 0;
}