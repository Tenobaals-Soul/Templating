#if !defined (TEST)
#define TEST
#endif
#if !defined(DEBUG)
#define DEBUG
#endif
#include <parser.h>
#include <test_core.h>
#include <stdio.h>

reader_t* mr(char* t) {
    reader_t* r = malloc(sizeof(reader_t));
    r[0] = (reader_t) {t, 0, 0, 0};
    return r;
}

void test_basics() {
    enviroment_t env;
    string_dict_init(env.operators);
    string_dict_put(env.operators, "+", NULL);
    assert_equal(parse_char(mr("hello world"), NULL), 'h');
    assert_equal(parse_specific_char(mr("231"), NULL, '_'), 0);
    assert_equal(parse_specific_char(mr("_231"), NULL, '_'), '_');
    assert_equal(parse_alpha(mr("_231"), NULL), 0);
    assert_equal(parse_alpha(mr("hello"), NULL), 'h');
    assert_equal(parse_digit(mr("hello world"), NULL), 0);
    assert_equal(parse_digit(mr("123"), NULL), '1');
    assert_equal(parse_punct(mr("hello world"), NULL), 0);
    assert_equal(parse_punct(mr("/"), NULL), '/');
    assert_equal(parse_number(mr("0.5"), NULL).f, 0.5);
    assert_equal((int) parse_number(mr("r0.5"), NULL).type, NNONE);
    assert_equal(parse_character(mr("'a'"), NULL), 'a');
    assert_equal(parse_character(mr("a'"), NULL), 0);
    assert_str_equal(parse_identifier(mr("hello world"), NULL), "hello");
    assert_str_equal(parse_operator(mr("+ 2"), NULL, &env), "+");
    assert_str_equal(parse_operator(mr("2times 2"), NULL, &env), NULL);
    assert_str_equal(parse_string(mr("\"hello world\""), NULL), "hello world");
    assert_str_equal(parse_string(mr("hello world"), NULL), NULL);
    string_dict_destroy(env.operators);
}

void test_value_parsing() {
    enviroment_t env;
    string_dict_init(env.operators);
    struct Exception* exc = NULL;
    value_char_t* vc = (void*) parse_value(mr("'a'e"), &exc, &env);
    if (vc == NULL) { print_exception(exc); exc = NULL; }
    else {
        assert_equal((int) vc->type, VALUE_CHAR);
        assert_equal(vc->value, 'a');
    }
    value_integer_t* vi = (void*) parse_value(mr("420e"), &exc, &env);
    if (vi == NULL) { print_exception(exc); exc = NULL; }
    else {
        assert_equal((int) vi->type, VALUE_INTEGER);
        assert_equal(vi->value, (uintmax_t) 420);
    }
    value_floating_t* vf = (void*) parse_value(mr("12.5"), &exc, &env);
    if (vf == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) vf->type, VALUE_FLOATING);
        assert_equal(vf->value, 12.5);
    }
    value_string_t* vs = (void*) parse_value(mr("\"hello\" + "), &exc, &env);
    if (vs == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) vs->type, VALUE_STRING);
        assert_str_equal(vs->value, "hello");
    }
    value_tuple_t* vt = (void*) parse_value(mr("(\"hello\", 2)"), &exc, &env);
    if (vt == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) vt->type, VALUE_TUPLE);
        assert_equal((int) *vt->value[0], VALUE_STRING);
        assert_equal((int) *vt->value[1], VALUE_INTEGER);
    }
    value_scalar_initializer_t* vv = (void*) parse_value(mr("{\"hello\", 2}"), &exc, &env);
    if (vv == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) vv->type, VALUE_SCALAR_INITIALIZER);
        assert_equal((int) *vv->value[0], VALUE_STRING);
        assert_equal((int) *vv->value[1], VALUE_INTEGER);
    }
    string_dict_destroy(env.operators);
}

void test_expression_parsing() {
    enviroment_t env;
    string_dict_init(env.operators);
    string_dict_put(env.operators, "+", NULL);
    string_dict_put(env.operators, "*", NULL);
    struct Exception* exc;
    value_operation_t* val;
    val = (void*) parse_expression(mr("5 + 7"), &exc, &env);
    if (val == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) val->type, VALUE_OPERATION);
    }
    
    val = (void*) parse_expression(mr("5 + 7 * 5"), &exc, &env);
    if (val == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) val->type, VALUE_OPERATION);
        assert_str_equal(val->operator_name, "+");
    }

    val = (void*) parse_expression(mr("(5 + 7) * 5"), &exc, &env);
    if (val == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) val->type, VALUE_OPERATION);
        assert_str_equal(val->operator_name, "*");
    }
    val = (void*) parse_expression(mr("(5 + 5"), NULL, &env);
    assert_equal(val, NULL);
    string_dict_destroy(env.operators);
}

int main() {
    start();
    launch(test_basics());
    launch(test_value_parsing());
    launch(test_expression_parsing());
    end();
}