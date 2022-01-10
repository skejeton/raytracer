#pragma once
#include "../deps/everything.h"
#include <stdio.h>
#include <math.h>
#include <errno.h>

typedef u32 argb;

struct sourced_error {
        const char *source;
        const char *alt;
        int errn;
}
typedef sourced_error;

#define MKSTR1(x) #x 
#define MKSTR2(x) MKSTR1(x)
#define MKSRC (__FILE__ ":" MKSTR2(__LINE__)) 
#define MKERR(x) (sourced_error) { MKSRC, NULL, -(x) }
#define MKALT(x, m) (sourced_error) { MKSRC, ("(" m ")"), -(x) }
#define MKOK (sourced_error) { MKSRC, NULL, 0 }

__attribute__((unused))
static inline argb f2argb(f64 a, f64 r, f64 g, f64 b) {
        a = fmin(fmax(a, 0.0), 1.0);
        r = fmin(fmax(r, 0.0), 1.0);
        g = fmin(fmax(g, 0.0), 1.0);
        b = fmin(fmax(b, 0.0), 1.0);

        u32 _a = (u32)(a * 255) & 0xFF;
        u32 _r = (u32)(r * 255) & 0xFF;
        u32 _g = (u32)(g * 255) & 0xFF;
        u32 _b = (u32)(b * 255) & 0xFF;

        return _a << 24 | _r << 16 | _g << 8 | _b << 0;
}

struct argb_buffer {
        argb *buf;
        usize width;
        usize height;
}
typedef argb_buffer;

__attribute__((unused))
static inline void errchk(sourced_error err) {
        if (err.errn < 0) {
                char buf[256] = { 0 };
                strerror_r(-err.errn, buf, 256);
                fprintf(stdout, 
                        "\x1b[31;1m[err]\x1b[0m\x1b[37m %s: %s %s\x1b[0m\n",
                        err.source, buf, err.alt
                );
                exit(-1);
        }
}

sourced_error make_image(argb_buffer *into, usize w, usize h);

__attribute__((unused))
inline void destroy_image(argb_buffer *self) {
        free(self->buf);
}

__attribute__((unused))
inline void image_pixelunc(argb_buffer *self, usize x, usize y, argb color) {
        self->buf[y*self->width+x] = color;
}

__attribute__((unused))
void image_clear(argb_buffer *self, argb value);

sourced_error ppm_write_image(argb_buffer *buffer, char const *path);

__attribute__((unused))
inline f64 random_double() {
        return rand() / (f64)(RAND_MAX + 1.0);
}

__attribute__((unused))
inline f64 random_double_of(f64 min, f64 max) {
        return min+random_double()*(max-min);
}
