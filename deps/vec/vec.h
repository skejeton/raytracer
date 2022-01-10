#pragma once

#include <stdbool.h>
#include <memory.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include "../slab/slab.h"

struct Vec {
        size_t size;
        struct Slab cont;
};

struct Vec vec_new(size_t etype);
void vec_push(struct Vec *vec, void *val);

__attribute__((unused))
static inline void *vec_get(struct Vec *vec, size_t at)
{
        if (at >= vec->size)
                return NULL;
        return slab_read(&vec->cont, at);
}
void vec_pop(struct Vec *vec);
void vec_drop(struct Vec *vec);
void vec_remove(struct Vec *vec, size_t at);
