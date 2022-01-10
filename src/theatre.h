#pragma once
#include "hit.h"
#include "camera.h"
#include "ray.h"

enum actor_kind {
        ACTOR_SPHERE
}
typedef actor_kind;

struct sphere_actor {
        point3 origin;
        f64 radius;
}
typedef sphere_actor;

enum material_kind {
        MATERIAL_DIELECTRIC,
        MATERIAL_LAMBERTIAN,
        MATERIAL_METALLIC
}
typedef material_kind;

struct material {
        material_kind kind;
        color albedo;
}
typedef material;

struct actor {
        actor_kind kind;
        material mat;
        union {
                sphere_actor sphere;
        } data;
}
typedef actor;

struct theatre {
        camera cam; 
        struct Vec OF(actor) actors;
}
typedef theatre;

static inline bool mat_scatter(
        material *self,
        const ray r,
        const hit *hit,
        color *attenuation,
        ray *scatter)
{
        switch (self->kind) {
        case MATERIAL_DIELECTRIC: {
                *scatter = ray_of(hit->at, v3_neg(r.dir));
                *attenuation = self->albedo;
                return true;
        } 
        case MATERIAL_LAMBERTIAN: {
                v3 dir = v3_add(hit->normal, v3_rand_unit_sphere());
                *scatter = ray_of(hit->at, dir);
                if (v3_near_zero(dir))
                        dir = hit->normal;
                *attenuation = self->albedo;
                return true;
        }
        case MATERIAL_METALLIC: {
                v3 dir = v3_reflect(v3_norm(r.dir), hit->normal);
                *scatter = ray_of(hit->at, dir);
                *attenuation = self->albedo;
                return v3_dot(scatter->dir, hit->normal) > 0.0;
        }
        }
}

theatre theatre_new(f64 aspect) {
        return (theatre) {
                .actors = vec_new(sizeof(actor)),
                .cam = camera_at(v3_of(0, 0.0, 0), aspect, 1.0)
        };
}

void theatre_drop(theatre *self) {
        vec_drop(&self->actors);
}

actor actor_sphere(material mat, point3 origin, f64 radius) {
        return (actor) { 
                .mat = mat,
                .kind = ACTOR_SPHERE,
                .data.sphere = { 
                        .origin = origin,
                        .radius = radius 
                } 
        };
}

void theatre_add(theatre *self, actor actor) {
        printf("Adding actor\n");
        vec_push(&self->actors, &actor);
}

static inline hit launch_ray(actor *actor, ray r) {
        switch (actor->kind) {
        case ACTOR_SPHERE: {
                sphere_actor *sphere = &actor->data.sphere;
                return ray_vs_sphere(r, sphere->origin, sphere->radius); 
        } break;
        }
}

static inline bool theatre_shoot(theatre *self, ray r, hit *dest, actor **hit) {
        bool hit_anything = false;
        for (usize i = 0; i < self->actors.size; i += 1) {
                actor *actor = vec_get(&self->actors, i);
                struct hit maybe_hit = launch_ray(actor, r);

                if (maybe_hit.t > 0)  {
                        if (hit_anything && maybe_hit.t > dest->t) continue;
                        hit_anything = true;
                        *hit = actor;
                        *dest = maybe_hit;
                }
        }

        return hit_anything;
}

static inline color theatre_ray(theatre *self, ray r, usize hits) {
        struct hit hit;
        
        actor *found_actor;

        if (hits == 0)
                return (color) { 0 };

        if (!theatre_shoot(self, r, &hit, &found_actor))
                return camera_ray_color(&self->cam, r);

        color attenuation;
        ray scattered;

        if (!mat_scatter(&found_actor->mat, r, &hit, &attenuation, &scattered)) 
                return (color) { 0 };

        v3 casted = theatre_ray(self, scattered, hits - 1);
        return v3_mulv(casted, attenuation);
}

__attribute__((unused))
static inline color theatre_sample(theatre *self, f64 u, f64 v) {
        return theatre_ray(self, camera_ray(&self->cam, u, v), 3);
}

