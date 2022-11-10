#ifndef INCLUDE_OPEN_TEMPLATE_H
#define INCLUDE_OPEN_TEMPLATE_H
#include <string_dict.h>

typedef union any (*filter_chr_type)(char);
typedef union any (*filter_int_type)(long);
typedef union any (*filter_flt_type)(double);
typedef union any (*filter_str_type)(const char*, size_t actual_lenght);
typedef union any (*filter_ptr_type)(void*);
typedef union any (*filter_boo_type)(bool);

typedef struct filter_set* filter_set_t;

#endif