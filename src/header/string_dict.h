#ifndef INCLUDE_STRING_DICT_H
#define INCLUDE_STRING_DICT_H
#include<stdbool.h>

#define STRING_DICT_TABLE_SIZE 512

typedef struct string_dict {
    struct string_dict_item* items;
    unsigned int capacity;
    unsigned int count;
} string_dict_t[1];

typedef struct dict_iter {
    unsigned int _i;
    struct string_dict* _dict;
    bool hasnext;
    char* key;
    void* val;
} dict_iter_t;

void string_dict_init(string_dict_t dict);
void string_dict_put(string_dict_t dict, const char* key, void* val);
void* string_dict_get(string_dict_t dict, const char* key);
unsigned int string_dict_get_size(string_dict_t dict);
void string_dict_destroy(string_dict_t dict);
void string_dict_foreach(string_dict_t dict, void (*action)(const char* key, void* val));
void string_dict_complex_foreach(string_dict_t dict, void (*action)
        (void* enviroment, const char* key, void* val), void* enviroment);
unsigned int string_dict_copy(string_dict_t dest, string_dict_t src);
unsigned int string_dict_copy_if(string_dict_t dest, string_dict_t src, bool (*condition)(const char* key, void* val));
unsigned int string_dict_copy_complex_if(string_dict_t dest, string_dict_t src, bool (*condition)
        (void* enviroment, const char* key, void* val), void* enviroment);

dict_iter_t get_dict_iter(string_dict_t dict);
void dict_iter_advance(dict_iter_t* i);

#endif