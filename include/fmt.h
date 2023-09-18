#ifndef FMT_H_FABRICATORZAYAC_
#define FMT_H_FABRICATORZAYAC_

#include <stdio.h>

typedef enum {
    FMT_OK,
    FMT_ERR,
} fmt_error;

typedef struct {
    void (*fmt)(const void *ctx, FILE *stream);
} Display_vtable;
typedef struct {
    const void *ptr;
    const Display_vtable *vtable;
} Display_t;

typedef struct {
    fmt_error (*print)(const char *restrict fmt, ...);
} fmt_mod;

extern const fmt_mod fmt;

#endif // !FMT_H_FABRICATORZAYAC_
