#pragma once
#include "ray.h"

struct hit {
        point3 at;
        v3 normal;
        bool front_face;
        f64 t;
};

typedef struct hit hit;

__attribute__((unused))
static inline hit hit_of(ray r, point3 at, v3 normal, f64 t) {
        bool front_face = v3_dot(r.dir, normal) < 0;
        return (hit) { 
                .front_face = front_face,
                .at = at,
                .normal = front_face ? normal : v3_neg(normal),
                .t = t
        };
}

__attribute__((unused))
static inline hit hit_at(ray r, f64 t, v3 normal) {
        return hit_of(r, ray_at(r, t), normal, t);
}

__attribute__((unused))
static inline hit ray_vs_sphere(ray r, point3 center, f64 radius) {
        v3 a_sub_c = v3_sub(r.origin, center);

        f64 a = v3_dot(r.dir, r.dir),
            b = 2.0*v3_dot(a_sub_c, r.dir),
            c = v3_dot(a_sub_c, a_sub_c)-radius*radius;

        f64 disc = (b*b - 4*a*c);    
        f64 t = disc < 0 ? -1 : (-b - sqrt(disc)) / (2*a);
        return hit_at(r, t, v3_div(v3_sub(ray_at(r, t), center), radius));
}   

