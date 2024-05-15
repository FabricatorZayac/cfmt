#ifndef FMT_H_FABRICATORZAYAC_
#define FMT_H_FABRICATORZAYAC_

#include <stdio.h>

typedef enum {
    GEN_END,
    GEN_INTERFACE,
    GEN_INT,
    GEN_DOUBLE,
    GEN_CSTR,
    GEN_ERR,
} fmt_marker_generic;

#ifdef CFMT_PRINT_MACROS
#include "cursed_macros.h"

#define _FMT_MARKER(ARG) _Generic((ARG), \
    int: GEN_INT, \
    double: GEN_DOUBLE, \
    const char *: GEN_CSTR, \
    fmt_error: GEN_ERR, \
    fmt_t: GEN_INTERFACE)

#define _FMT_WITH_MARKER(ARG) _FMT_MARKER(ARG), ARG,

// Add markers to argc
#define format(STREAM, FMT, ...) \
    _format( \
        STREAM, \
        FMT, \
        __VA_OPT__(FOREACH(_FMT_WITH_MARKER, __VA_ARGS__)) 0)

#define format_or_die(STREAM, FMT, ...) \
    _format_or_die( \
        STREAM, \
        FMT, \
        __VA_OPT__(FOREACH(_FMT_WITH_MARKER, __VA_ARGS__)) 0)

/**
 * @description Safe print function
 * @arg FMT format string
 * @arg ... stuff you wanna print
 * @return fmt_error [[nodiscard]]
 **/
#define print(FMT, ...) format(stdout, FMT, __VA_ARGS__)

/**
 * @description Unsafe print function. Same as the safe one, but dies on error
 **/
#define printu(FMT, ...) format_or_die(stdout, FMT, __VA_ARGS__)
#endif

typedef enum {
    FMT_OK,
    FMT_ERR_FPRINTF,
    FMT_ERR_NO_OPENBRACKET,
    FMT_ERR_NO_CLOSEBRACKET,
    FMT_ERR_NOT_ENOUGH_ARGS,
} fmt_error;

#ifdef __has_c_attribute
#define NODISCARD [[nodiscard]]
#endif

typedef struct {
    const void *ptr;
    NODISCARD fmt_error (*fmt)(const void *ctx, FILE *stream);
} fmt_t;

typedef struct {
    NODISCARD fmt_error (*_format)(FILE *stream, const char *restrict fmt, ...);
    void (*_format_or_die)(FILE *stream, const char *restrict fmt, ...);
    fmt_t (*Int)(int *self);
    fmt_t (*Double)(double *self);
    fmt_t (*CStr)(const char *self);
    fmt_t (*errstr)(fmt_error self);
} fmt_mod;

#undef NODISCARD

extern const fmt_mod fmt;


#define FMT_REPORT(ERR) report_error(__FILE__, __LINE__, ERR)
#define FMT_REPORT_AND_DIE(ERR) do { \
    FMT_REPORT(ERR);                 \
    exit(ERR);                       \
} while (0)

static inline void report_error(const char *file, int line, fmt_error err) {
    (void)fmt._format(
        stderr,
        "{}:{}: error: {}\n",
        GEN_CSTR, file,
        GEN_INT, line,
        GEN_ERR, err,
    0);
}

#endif // !FMT_H_FABRICATORZAYAC_
