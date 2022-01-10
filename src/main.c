#include <stdio.h>
#include <math.h>
#include "theatre.h"
#include "things.h"
#include "v3.h"
#include <pthread.h>

const usize SAMPLE_COUNT = 10;

struct render_region {
        bool local;
        struct argb_buffer *image;
        theatre *scene;
        usize y_offs;
        usize y_size;
};

color samples(theatre *scene, usize x, usize y, usize iw, usize ih) {
        color col = v3_of(0, 0, 0); 

        for (usize i = 0; i < SAMPLE_COUNT; i += 1) {
                f64 u = ((f64)x+random_double()/3.0) / (f64)(iw-1);
                f64 v = ((f64)y+random_double()/3.0) / (f64)(ih-1);
                col = v3_add(col, theatre_sample(scene, u, v));
        }

        return v3_div(col, (f64) SAMPLE_COUNT);
}

void *render_part(void *_region) {
        struct render_region *region = _region; 
        struct argb_buffer *image = region->image; 
        printf(
                "Starting thread for ( %zu : %zu )\n", 
                region->y_offs, region->y_size
        );
        usize region_end = (region->y_offs+region->y_size);
        for (usize _y = 0; _y < image->height; _y += 1) {
                // Invert
                usize y = image->height-1-_y;
                for (usize x = region->y_offs; x < region_end; x += 1) {
                        color col = samples(
                                region->scene, 
                                x, y, image->width, image->height
                        );
                        argb screen_color = f2argb(1.0, col.x, col.y, col.z);
                        image_pixelunc(image, x, _y, screen_color);
                }
        }
        printf("Finishing for ( %zu : %zu )\n", region->y_offs, region->y_size);
        if (!region->local) pthread_exit(NULL);
        return NULL;
}



int main() {
        struct argb_buffer image;
        f64 aspect = 1.0/1.0,
            width  = 800,
            height = 600;

        errchk(make_image(&image, (usize)width, (usize)height));
        image_clear(&image, 0xFF000000);


        theatre theatre = theatre_new(aspect);

        // Ground
        theatre_add(&theatre, actor_sphere((material) {
                .albedo = v3_of(0.7, 0.7, 0.7),
                .kind = MATERIAL_LAMBERTIAN
        }, v3_of(3.0, -1000.5, -1), 1000));


        theatre_add(&theatre, actor_sphere((material) {
                .albedo = v3_of(1.0, 1.0, 1.0),
                .kind = MATERIAL_METALLIC
        }, v3_of(-1/3.0*2, 0.0, -1), 1/3.0));
        theatre_add(&theatre, actor_sphere((material) {
                .albedo = v3_of(1.0, 0.5, 0.5),
                .kind = MATERIAL_LAMBERTIAN
        }, v3_of(0/3.0, 0.0, -1), 1/3.0));
        theatre_add(&theatre, actor_sphere((material) {
                .albedo = v3_of(1.0, 1.0, 1.0),
                .kind = MATERIAL_DIELECTRIC
        }, v3_of(1/3.0*2, 0.0, -1), 1/3.0));


        /*
        for (usize i = 0; i < 10; i += 1) {
                v3 pos = v3_of(
                        (f64)(i%15)-5+random_double_of(-0.1, 0.1),
                        -0.5+random_double_of(-0.05, 2.0),
                        -(f64)(i/15)+random_double_of(-0.1, 0.1)
                );

                f64 radius = 0.3+random_double_of(-0.05, 0.2);
                theatre_add(&theatre, actor_sphere(pos, radius));
        }
        */
        const usize THREAD_N = 16;

        pthread_t threads[THREAD_N];
        int ret[THREAD_N];
        struct render_region regions[THREAD_N];

        for (usize i = 0; i < THREAD_N-1; i += 1) {
                regions[i] = (struct render_region) {
                        .local = false,
                        .image = &image,
                        .scene = &theatre,
                        .y_offs = (usize)((width/(f64)THREAD_N)*(f64)i),
                        .y_size = (usize)(width/(f64)THREAD_N)
                };
                if (pthread_create(&threads[i], 0, render_part, &regions[i])) {
                        errchk(MKERR(errno));
                }
        }

        // Start on the main thread too, why not
        render_part(&(struct render_region) {
                .local = true,
                .image = &image,
                .scene = &theatre,
                .y_offs = (usize)((width/(f64)THREAD_N)*(f64)(THREAD_N-1)),
                .y_size = (usize)(width/(f64)THREAD_N)
        });


        for (usize i = 0; i < THREAD_N-1; i += 1) {
                pthread_join(threads[i], NULL);
                printf("Thread %zu done (%d)\n", i, ret[i]);
        }


        theatre_drop(&theatre);
        errchk(ppm_write_image(&image, "./output.ppm"));
        destroy_image(&image);
        printf("DONE!\n");
}
