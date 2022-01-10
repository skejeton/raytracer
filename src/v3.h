#pragma once
#include "../deps/everything.h"
#include "things.h"
#include <math.h>
#include <stdio.h>

struct v3 {
        f64 x, y, z;
};

typedef struct v3 v3;
typedef v3 point3;
typedef v3 color;
        
// Zero-filled v3
__attribute__((unused))
static inline v3 v3_zero() {
        return (v3) { 0, 0, 0 };
}

// v3 of certain components
__attribute__((unused))
static inline v3 v3_of(f64 x, f64 y, f64 z) {
        return (v3) { x, y, z };
}

// Random vector in range
__attribute__((unused))
static inline v3 v3_rand_of(f64 min, f64 max) {
        return (v3) {
                random_double_of(min, max),
                random_double_of(min, max),
                random_double_of(min, max)
        };
}

// Negates v3
__attribute__((unused))
static inline v3 v3_neg(v3 of) {
        return (v3) { -of.x, -of.y, -of.z };
}

// Sums v3
__attribute__((unused))
static inline v3 v3_add(v3 a, v3 b) {
        return (v3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

// Sums v3
__attribute__((unused))
static inline v3 v3_sub(v3 a, v3 b) {
        return v3_add(a, v3_neg(b));
}

// Multiplies v3
__attribute__((unused))
static inline v3 v3_mul(v3 v, f64 by) {
        return (v3) { v.x*by, v.y*by, v.z*by };
}

// Multiplies v3 by v3
__attribute__((unused))
static inline v3 v3_mulv(v3 a, v3 b) {
        return (v3) { a.x*b.x, a.y*b.y, a.z*b.z };
}

// Divides v3
__attribute__((unused))
static inline v3 v3_div(v3 v, f64 by) {
        return (v3) { v.x/by, v.y/by, v.z/by };
}

// Length squared
__attribute__((unused))
static inline f64 v3_lensq(v3 of) {
        return of.x*of.x+of.y*of.y+of.z*of.z;
}

// Length
__attribute__((unused))
static inline f64 v3_len(v3 of) {
        return sqrt(v3_lensq(of));
}

// Normalize
__attribute__((unused))
static inline v3 v3_norm(v3 of) {
        return v3_div(of, v3_len(of));
}

// Dot
__attribute__((unused))
static inline f64 v3_dot(v3 a, v3 b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Cross
__attribute__((unused))
static inline v3 v3_cross(v3 a, v3 b) {
        return v3_of(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
        ); 
}

__attribute__((unused))
static inline void v3_printf(v3 v) {
        printf("v3(%lg, %lg, %lg)", v.x, v.y, v.z);
}

__attribute__((unused))
static inline v3 v3_rand_unit_sphere() {
        return v3_norm(v3_rand_of(-1, 1));
}

__attribute__((unused))
static inline v3 v3_rand_hemisphere(v3 normal) {
        v3 i = v3_rand_unit_sphere();
        if (v3_dot(i, normal) > 0.0)
                return i;
        return v3_neg(i);
} 

__attribute__((unused))
static inline bool v3_near_zero(v3 v) {
        const f64 E = 1e-8;
        return (fabs(v.x) < E) && (fabs(v.y) < E) && (fabs(v.z) < E);
}

__attribute__((unused))
static inline v3 v3_reflect(v3 dir, v3 normal) {
        return v3_sub(dir, v3_mul(normal, 2.0*v3_dot(dir, normal)));
}
