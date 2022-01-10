#pragma once
#include "v3.h"

struct ray {
        point3 origin;
        v3 dir;
};

typedef struct ray ray;

__attribute__((unused))
static inline ray ray_of(v3 origin, v3 dir) {
        return (ray) { .origin = origin, .dir = dir };
}

__attribute__((unused))
static inline point3 ray_at(ray r, f64 offs) {
        return v3_add(r.origin, v3_mul(r.dir, offs));
}

