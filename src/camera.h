#pragma once
#include "ray.h"

struct camera {
        f64 focal, viewport_height, viewport_width;
        color sky, fade;
        v3 origin;
};

typedef struct camera camera;

__attribute__((unused))
static inline camera camera_at(v3 origin, f64 aspect, f64 focal) {
        return (camera) { 
                .focal = focal, 
                .viewport_width = 2.0*aspect, 
                .sky = v3_of(0.4, 0.5, 0.7),
                .fade = v3_of(0.7, 0.7, 0.8),
                .viewport_height = 2.0,
                .origin = origin
        };
}

static inline v3 camera_uv(camera *cam, f64 u, f64 v) {
        v3 horizontal  = v3_of(cam->viewport_width,  0, 0),
           vertical    = v3_of(0, cam->viewport_height, 0);
        return v3_add(v3_mul(horizontal, u), v3_mul(vertical, v));
}

__attribute__((unused))
static inline ray camera_ray(camera *cam, f64 u, f64 v) {
        v3 xy          = v3_sub(cam->origin, camera_uv(cam, 0.5, 0.5)),
           bottom_left = v3_sub(xy, v3_of(0, 0, cam->focal)),
           at          = v3_add(bottom_left, camera_uv(cam, u, v));

        return ray_of(cam->origin, v3_sub(at, cam->origin));
}

__attribute__((unused))
static inline color camera_ray_color(camera *cam, ray ray) {
        // ray.dir.y -= 1.0;
        v3 unit = v3_norm(ray.dir);
        f64 t = 0.5*(unit.y+1.0);
        return v3_add(v3_mul(cam->sky, t), v3_mul(cam->fade, 1-t));
}
