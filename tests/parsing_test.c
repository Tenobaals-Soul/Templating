#if !defined (TEST)
#define TEST
#endif
#if !defined(DEBUG)
#define DEBUG
#endif
#include <parser.h>
#include <test_core.h>
#include <stdio.h>
#include <io_queues.h>

reader_t* mr(char* t) {
    reader_t* r = malloc(sizeof(reader_t));
    r[0] = (reader_t) {make_simple_string_input_queue(t), 0};
    return r;
}

void test_basics() {
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
    assert_equal(parse_operator(mr("+ 2"), NULL), ADD);
    assert_equal(parse_operator(mr("2times 2"), NULL), -1);
    assert_str_equal(parse_string(mr("\"hello world\""), NULL, NULL), "hello world");
    assert_str_equal(parse_string(mr("hello world"), NULL, NULL), NULL);
}

void test_value_parsing() {
    struct Exception* exc = NULL;
    value_char_t* vc = (void*) parse_value(mr("'a'e"), &exc);
    if (vc == NULL) { print_exception(exc); exc = NULL; }
    else {
        assert_equal((int) vc->type, VALUE_CHAR);
        assert_equal(vc->value, 'a');
    }
    value_integer_t* vi = (void*) parse_value(mr("420e"), &exc);
    if (vi == NULL) { print_exception(exc); exc = NULL; }
    else {
        assert_equal((int) vi->type, VALUE_INTEGER);
        assert_equal(vi->value, (uintmax_t) 420);
    }
    value_floating_t* vf = (void*) parse_value(mr("12.5"), &exc);
    if (vf == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) vf->type, VALUE_FLOATING);
        assert_equal(vf->value, 12.5);
    }
    value_string_t* vs = (void*) parse_value(mr("\"hello\" + "), &exc);
    if (vs == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) vs->type, VALUE_STRING);
        assert_str_equal(vs->value, "hello");
    }
}

void test_expression_parsing() {
    struct Exception* exc;
    value_operation_t* val;
    val = (void*) parse_expression(mr("5 + 7"), &exc);
    if (val == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) val->type, VALUE_OPERATION);
    }
    
    val = (void*) parse_expression(mr("5 + 7 * 5"), &exc);
    if (val == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) val->type, VALUE_OPERATION);
        assert_equal(val->operatorno, ADD);
    }

    val = (void*) parse_expression(mr("(5 + 7) * 5"), &exc);
    if (val == NULL) { print_exception(exc); exc = NULL; assert_true(false); }
    else {
        assert_equal((int) val->type, VALUE_OPERATION);
        assert_equal(val->operatorno, MULTIPLY);
    }
    val = (void*) parse_expression(mr("(5 + 5"), NULL);
    assert_equal(val, NULL);
}

int main() {
    start();
    launch(test_basics());
    launch(test_value_parsing());
    launch(test_expression_parsing());
    end();
}