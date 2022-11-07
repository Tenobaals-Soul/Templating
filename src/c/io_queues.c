#include <io_queues.h>
#include <stdlib.h>
#include <stack.h>
#include <string.h>

struct output_queue_virtual_table {
    void (*pushimpl)(output_queue_t self, char c);
    void (*pushmanyimpl)(output_queue_t self, const char* c, size_t bytes);
};

struct output_queue {
    const struct output_queue_virtual_table* vt;
};

void ssoq_push(output_queue_t self, char c) {
    struct stack* stack = (struct stack*) (self + 1);
    push_chr(stack, c);
}

void ssoq_pushmany(output_queue_t self, const char* c, size_t bytes) {
    struct stack* stack = (struct stack*) (self + 1);
    for (size_t i = 0; i < bytes; i++) {
        push_chr(stack, c[i]);
    }
}

struct output_queue_virtual_table simple_string_output_queue_vt = {
    ssoq_push,
    ssoq_pushmany
};

output_queue_t make_simple_string_output_queue() {
    output_queue_t oq = malloc(sizeof(struct output_queue) + sizeof(stack_t));
    oq->vt = &simple_string_output_queue_vt;
    init_stack((struct stack*) (oq + 1));
    return oq;
}

struct input_queue_virtual_table {
    void (*seekimpl)(input_queue_t self, size_t bytes);
    int (*peekimpl)(input_queue_t self);
    size_t (*peekmanyimpl)(input_queue_t self, size_t bytes, char* buffer);
    int (*popimpl)(input_queue_t self);
    size_t (*popmanyimpl)(input_queue_t self, size_t bytes, char* buffer);
    void (*skipimpl)(input_queue_t self);
    size_t (*skipmanyimpl)(input_queue_t self, size_t bytes);
};

struct input_queue {
    const struct input_queue_virtual_table* vt;
    size_t cur;
};

struct simple_string_input_queue_data {
    const char* src;
};

void ssiq_seek(input_queue_t self, size_t bytes) {
    self->cur = bytes;
}

int ssiq_peek(input_queue_t self) {
    struct simple_string_input_queue_data* qd = (void*) (self + 1);
    return qd->src[self->cur] == 0 ? -1 : qd->src[self->cur];
}

size_t ssiq_peekmany(input_queue_t self, size_t bytes, char* buffer) {
    struct simple_string_input_queue_data* qd = (void*) (self + 1);
    size_t i;
    const char* src = qd->src + self->cur;
    for (i = 0; i < bytes && src[i]; i++) {
        buffer[i] = src[i];
    }
    buffer[i] = '\0';
    return i;
}

int ssiq_pop(input_queue_t self) {
    struct simple_string_input_queue_data* qd = (void*) (self + 1);
    return qd->src[self->cur] == 0 ? -1 : qd->src[self->cur++];
}

size_t ssiq_popmany(input_queue_t self, size_t bytes, char* buffer) {
    struct simple_string_input_queue_data* qd = (void*) (self + 1);
    size_t i;
    const char* src = qd->src + self->cur;
    for (i = 0; i < bytes && src[i]; i++) {
        buffer[i] = ssiq_pop(self);
    }
    buffer[i] = '\0';
    return i;
}

void ssiq_skip(input_queue_t self) {
    struct simple_string_input_queue_data* qd = (void*) (self + 1);
    if (qd->src[self->cur] != 0) self->cur++;
}

size_t ssiq_skipmany(input_queue_t self, size_t bytes) {
    struct simple_string_input_queue_data* qd = (void*) (self + 1);
    size_t i;
    const char* src = qd->src + self->cur;
    for (i = 0; i < bytes && src[i]; i++) {
        self->cur++;
    }
    return i;
}

struct input_queue_virtual_table simple_string_input_queue_vt = {
    ssiq_seek,
    ssiq_peek,
    ssiq_peekmany,
    ssiq_pop,
    ssiq_popmany,
    ssiq_skip,
    ssiq_skipmany
};

input_queue_t make_simple_string_input_queue(const char* src) {
    input_queue_t oq = malloc(sizeof(struct input_queue) + sizeof(stack_t));
    struct simple_string_input_queue_data* qd = (void*) (oq + 1);
    oq->vt = &simple_string_input_queue_vt;
    oq->cur = 0;
    qd->src = src;
    return oq;
}

void pushqueue(output_queue_t queue, char c) {
    return queue->vt->pushimpl(queue, c);
}

void pushqueuemany(output_queue_t queue, const char* c, size_t bytes) {
    return queue->vt->pushmanyimpl(queue, c, bytes);
}

size_t getqueuepos(input_queue_t queue) {
    return queue->cur;
}

void seekqueue(input_queue_t queue, size_t bytes) {
    return queue->vt->seekimpl(queue, bytes);
}

int peekqueue(input_queue_t queue) {
    return queue->vt->peekimpl(queue);
}

size_t peekqueuemany(input_queue_t queue, size_t bytes, char* buffer) {
    return queue->vt->peekmanyimpl(queue, bytes, buffer);
}

int popqueue(input_queue_t queue) {
    return queue->vt->popimpl(queue);
}

size_t popqueuemany(input_queue_t queue, size_t bytes, char* buffer) {
    return queue->vt->popmanyimpl(queue, bytes, buffer);
}
void skipqueue(input_queue_t queue) {
    return queue->vt->skipimpl(queue);
}

size_t skipqueuemany(input_queue_t queue, size_t bytes) {
    return queue->vt->skipmanyimpl(queue, bytes);
}