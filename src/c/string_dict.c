#include<string_dict.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

struct string_dict_item {
    char* key;
    void* val;
    unsigned int raw_hash;
};

void string_dict_init(string_dict_t dict) {
    dict->count = 0;
    dict->capacity = STRING_DICT_TABLE_SIZE;
    dict->items = calloc(1, sizeof(struct string_dict_item) * dict->capacity);
}

static int hash_string(const char* string) {
    int hash = 0;
    for (unsigned int i = 0; i < 8 && string[i]; i++) {
        hash += string[i];
    }
    return hash;
}

static inline void string_dict_set(struct string_dict_item* array,
                                   unsigned int capacity,
                                   unsigned int hash,
                                   struct string_dict_item new) {
    for (unsigned int i = hash; i < capacity; i++) {
        if (array[i].key == NULL || strcmp(array[i].key, new.key) == 0) {
            array[i] = new;
            return;
        }
    };
    for (unsigned int i = 0; i < hash; i++) {
        if (array[i].key == NULL || strcmp(array[i].key, new.key) == 0) {
            array[i] = new;
            return;
        }
    };
    assert(false);
}

static void realloc_dict(string_dict_t dict) {
    struct string_dict_item* new_items = calloc(1, 
        sizeof(struct string_dict_item) *
        dict->capacity + STRING_DICT_TABLE_SIZE
    );
    unsigned int i = dict->capacity;
    do {
        i--;
        struct string_dict_item item = dict->items[i];
        string_dict_set(new_items, dict->capacity + STRING_DICT_TABLE_SIZE,
                        item.raw_hash % dict->capacity, item);
    } while(i);
    free(dict->items);
    dict->items = new_items;
    dict->capacity += STRING_DICT_TABLE_SIZE;
}

void string_dict_put(string_dict_t dict, const char* key, void* val) {
    if (dict->count == dict->capacity) realloc_dict(dict);
    unsigned int raw_hash = hash_string(key);
    unsigned int hash = raw_hash % dict->capacity;
    string_dict_set(dict->items, dict->capacity, hash,
                    (struct string_dict_item) {strdup(key), val, raw_hash});
    dict->count++;
}

void* string_dict_get(string_dict_t dict, const char* key) {
    unsigned int hash = ((unsigned) hash_string(key)) % dict->capacity;
    for (unsigned int i = hash; i < dict->capacity; i++) {
        if (dict->items[i].key == NULL) {
            return NULL;
        }
        if (strcmp(dict->items[i].key, key) == 0) {
            return dict->items[i].val;
        }
    };
    for (unsigned int i = 0; i < dict->capacity; i++) {
        if (dict->items[i].key == NULL) {
            return NULL;
        }
        if (strcmp(dict->items[i].key, key) == 0) {
            return dict->items[i].val;
        }
    };
    return NULL;
}

unsigned int string_dict_get_size(string_dict_t dict) {
    return dict->count;
}

void string_dict_destroy(string_dict_t dict) {
    unsigned int i = dict->capacity;
    if (i)
    do {
        i--;
        char* key = dict->items[i].key;
        if (key) free(key);
    } while(i);
    if (dict->items) free(dict->items);
}

void string_dict_foreach(string_dict_t dict, void (*action)(const char* key, void* val)) {
    unsigned int i = dict->capacity;
    if (i)
    do {
        i--;
        if (dict->items[i].key) {
            action(dict->items[i].key, dict->items[i].val);
        }
    } while(i);
}

void string_dict_complex_foreach(string_dict_t dict, void (*action)
        (void* enviroment, const char* key, void* val), void* enviroment) {
    unsigned int i = dict->capacity;
    if (i)
    do {
        i--;
        if (dict->items[i].key) {
            action(enviroment, dict->items[i].key, dict->items[i].val);
        }
    } while(i);
}

struct copy_env {
    string_dict_t dest;
    unsigned int counter;
    void* enviroment;
    union {
        bool (*condition)(const char* key, void* val);
        bool (*complex_condition)(void* enviroment, const char* key, void* val);
    };
};

static void string_dict_copy_internal(void* enviroment, const char* key, void* val) {
    struct copy_env* env = enviroment;
    string_dict_put(env->dest, key, val);
    env->counter++;
}

static void string_dict_copy_if_internal(void* enviroment, const char* key, void* val) {
    struct copy_env* env = enviroment;
    if (env->condition(key, val)) {
        string_dict_put(env->dest, key, val);
        env->counter++;
    }
}

static void string_dict_copy_complex_if_internal(void* enviroment, const char* key, void* val) {
    struct copy_env* env = enviroment;
    if (env->complex_condition(env->enviroment, key, val)) {
        string_dict_put(env->dest, key, val);
        env->counter++;
    }
}

unsigned int string_dict_copy(string_dict_t dest, string_dict_t src) {
    struct copy_env env = {{ dest[0] }, 0, NULL, {NULL}};
    string_dict_complex_foreach(src, string_dict_copy_internal, &env);
    return env.counter;
}

unsigned int string_dict_copy_if(string_dict_t dest, string_dict_t src, bool (*condition)(const char* key, void* val)) {
    struct copy_env env = {{ dest[0] }, 0, NULL, {.condition = condition}};
    string_dict_complex_foreach(src, string_dict_copy_if_internal, &env);
    return env.counter;
}

unsigned int string_dict_copy_complex_if(string_dict_t dest, string_dict_t src, bool (*condition)
        (void* enviroment, const char* key, void* val), void* enviroment) {
    struct copy_env env = {{ dest[0] }, 0, enviroment, {.complex_condition = condition}};
    string_dict_complex_foreach(src, string_dict_copy_complex_if_internal, &env);
    return env.counter;
}

dict_iter_t get_dict_iter(string_dict_t dict) {
    dict_iter_t iter = {
        ._dict = dict,
        ._i = -1,
        .hasnext = true,
        .key = NULL,
        .val = NULL
    };
    dict_iter_advance(&iter);
    return iter;
}

void dict_iter_advance(dict_iter_t* i) {
    do {
        i->_i++;
    } while(i->_i < i->_dict->capacity && i->_dict->items[i->_i].key == NULL);
    if (i->_i >= i->_dict->capacity) {
        i->_i--;
        i->hasnext = false;
        i->key = NULL;
        i->val = NULL;
    }
    else {
        i->hasnext = true;
        i->key = i->_dict->items[i->_i].key;
        i->val = i->_dict->items[i->_i].val;
    }
}