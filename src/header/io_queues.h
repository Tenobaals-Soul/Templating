#ifndef INCLUDE_IO_QUEUES_H
#define INCLUDE_IO_QUEUES_H
#define __need_size_t
#define __need_NULL
#include <stddef.h>

typedef struct input_queue* input_queue_t;
typedef struct output_queue* output_queue_t;

void pushqueue(output_queue_t queue, char c);
void pushqueuemany(output_queue_t queue, const char* c, size_t bytes);

size_t getqueuepos(input_queue_t queue);
void seekqueue(input_queue_t queue, size_t bytes);
int peekqueue(input_queue_t queue);
size_t peekqueuemany(input_queue_t queue, size_t bytes, char* buffer);
int popqueue(input_queue_t queue);
size_t popqueuemany(input_queue_t queue, size_t bytes, char* buffer);
void skipqueue(input_queue_t queue);
size_t skipqueuemany(input_queue_t queue, size_t bytes);

input_queue_t make_simple_string_input_queue(const char* src);

#endif