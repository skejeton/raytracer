#include "things.h"

sourced_error make_image(struct argb_buffer *into, usize w, usize h) {
        argb *buf = calloc(sizeof(argb), w*h);
        if (buf == NULL) return MKERR(ENOMEM);
        *into = (struct argb_buffer) {
                .buf = buf,
                        .width = w,
                        .height = h
        };
        return MKOK;
}

void image_clear(struct argb_buffer *self, argb value) {
        for (usize i = 0; i < (self->width*self->height); i += 1) {
                self->buf[i] = value; 
        }
}

sourced_error ppm_write_image(struct argb_buffer *buffer, char const *path) {
        FILE *f = fopen(path, "w+");

        if (f == NULL) 
                return MKALT(EINVAL, "Can't create file");

        // image too big
        if ((buffer->width*buffer->height) > 5000000) 
                return MKALT(ERANGE, "Image is too big");

        fprintf(f, "P3\n%zu %zu\n255\n", buffer->width, buffer->height);
        for (usize i = 0; i < (buffer->width*buffer->height); i += 1) {
                fprintf(f, "%u %u %u\n", 
                        (buffer->buf[i] >> 16) & 0xFF,
                        (buffer->buf[i] >>  8) & 0xFF,
                        (buffer->buf[i] >>  0) & 0xFF
                );
        }

        fclose(f);

        return MKOK;
}
