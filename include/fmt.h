#ifndef FMT_H_FABRICATORZAYAC_
#define FMT_H_FABRICATORZAYAC_

#include <stdio.h>

#include "cursed_macros.h"

// little (slightly cursed) wrapper macro that adds argc to the fmt.print function call
#define print(FMT, ...) print(FMT, NUM_ARGS(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__)
#define fprint(STREAM, FMT, ...) fprint(STREAM, FMT, NUM_ARGS(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__)

typedef enum {
    FMT_OK,
    FMT_ERR,
} fmt_error;

typedef struct {
    const void *ptr;
    fmt_error (*fmt)(const void *ctx, FILE *stream);
} fmt_t;

typedef struct {
    fmt_error (*print)(const char *restrict fmt, int argc, ...);
    fmt_error (*fprint)(FILE *stream, const char *restrict fmt, int argc, ...);
    fmt_t (*Int)(int *self);
    fmt_t (*Double)(double *self);
    fmt_t (*CStr)(const char *self);
} fmt_mod;

extern const fmt_mod fmt;

#endif // !FMT_H_FABRICATORZAYAC_
