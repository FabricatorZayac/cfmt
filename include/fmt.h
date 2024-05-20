#ifndef FMT_INCLUDE_FABRICATORZAYAC_H
#define FMT_INCLUDE_FABRICATORZAYAC_H

#include <stdio.h>
#include "cursed_macros.h"

typedef enum {
    GEN_END,
    GEN_INTERFACE,
    GEN_INT,
    GEN_DOUBLE,
    GEN_CSTR,
    GEN_ERR,
    GEN_BOOL,
#ifdef GEN_MIXIN
#define fmt_mixin(T, GEN, F) GEN,
    GEN_MIXIN
#undef fmt_mixin
#endif
} fmt_marker_generic;

#include "cursed_macros.h"

#define fmt_mixin(T, GEN, F) T: GEN,

#define _FMT_MARKER(ARG) _Generic((ARG), \
    GEN_MIXIN                            \
    bool: GEN_BOOL,                      \
    int: GEN_INT,                        \
    double: GEN_DOUBLE,                  \
    const char *: GEN_CSTR,              \
    char *: GEN_CSTR,                    \
    fmt_t: GEN_INTERFACE)

#define _FMT_WITH_MARKER(ARG) _FMT_MARKER(ARG), ARG,

// Add markers to argc
#define format(STREAM, FMT, ...) \
    _format(                     \
        STREAM,                  \
        FMT"",                   \
        __VA_OPT__(              \
        FOREACH(_FMT_WITH_MARKER, __VA_ARGS__) \
    ) 0)

#define format_or_die(STREAM, FMT, ...) \
    _format_or_die(                     \
        STREAM,                         \
        FMT"",                          \
        __FILE__, __LINE__,             \
        __VA_OPT__(FOREACH(_FMT_WITH_MARKER, __VA_ARGS__)) 0)

/**
 * @description Safe print function
 * @arg FMT format string literal
 * @arg ... stuff you wanna print
 * @return fmt_error
 **/
#define print(FMT, ...) format(stdout, FMT, __VA_ARGS__)

/**
 * @description Unsafe print function. Same as the safe one, but dies on error
 **/
#define printu(FMT, ...) format_or_die(stdout, FMT, __VA_ARGS__)
#define println(FMT, ...) printu(FMT"\n", __VA_ARGS__)


#ifdef __has_c_attribute
#define NODISCARD [[nodiscard]]
#endif

typedef enum NODISCARD {
    FMT_OK,
    FMT_ERR_FPRINTF,
    FMT_ERR_FWRITE,
    FMT_ERR_NO_OPENBRACKET,
    FMT_ERR_NO_CLOSEBRACKET,
    FMT_ERR_NOT_ENOUGH_ARGS,
} fmt_error;

#undef NODISCARD

typedef struct {
    const void *ptr;
    fmt_error (*fmt)(const void *ctx, FILE *stream);
} fmt_t;

#include <stdbool.h>

typedef struct {
    fmt_error (*_format)(FILE *stream, const char *restrict fmt, ...);
    void (*_format_or_die)(FILE *stream, const char *restrict fmt, const char *file, int line, ...);
    fmt_t (*Int)(int *self);
    fmt_t (*Double)(double *self);
    fmt_t (*CStr)(const char *self);
    fmt_t (*Bool)(bool self);
    fmt_t (*errstr)(fmt_error self);
} fmt_mod;
extern const fmt_mod fmt;

#define FMT_REPORT(ERR) report_error(__FILE__, __LINE__, ERR)
#define FMT_REPORT_AND_DIE(ERR) do { \
    FMT_REPORT(ERR);                 \
    exit(ERR);                       \
} while (0)

extern void report_error(const char *file, int line, fmt_error err);

#endif // !FMT_INCLUDE_FABRICATORZAYAC_H

// #define CFMT_IMPLEMENTATION // NOTE: For lsp purposes. Comment this line before building
#ifdef CFMT_IMPLEMENTATION

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INTERNAL(FN) ___internal_cfmt_##FN

void report_error(const char *file, int line, fmt_error err) {
    (void)fmt._format(
        stderr,
        "{}:{}: error: {}\n",
        // "{}:{}: \033[31;1;1merror\033[0m: {}\n",
        GEN_CSTR, file,
        GEN_INT, line,
        GEN_ERR, err,
        GEN_END
    );
}


fmt_error INTERNAL(vformat)(FILE *stream, const char *format, va_list argv) {
    // int arg_state = 0;
    fmt_marker_generic arg_state;
    size_t fmt_len = strlen(format);
    for (size_t i = 0; i < fmt_len;) {
        const size_t start_index = i;

        for (; i < fmt_len; i++) {
            switch (format[i]) {
                case '{':
                case '}':
                    goto _break;
            }
        }
        _break:;

        size_t end_index = i;
        bool unescape_brace = false;

        if (i + 1 < fmt_len && format[i + 1] == format[i]) {
            unescape_brace = true;
            end_index += 1;
            i += 2;
        }

        if (start_index != end_index) {
            if (fwrite(
                    format + start_index,
                    end_index - start_index,
                    1,
                    stream
                ) != 1) return FMT_ERR_FWRITE;
        }

        if (unescape_brace) continue;
        if (i >= fmt_len) break;

        if (format[i] == '}') return FMT_ERR_NO_OPENBRACKET;
        assert(format[i] == '{');
        i += 1;

        for (; i< fmt_len && format[i] != '}'; i++) {}
        if (i >= fmt_len) return FMT_ERR_NO_CLOSEBRACKET;
        assert(format[i] == '}');
        i += 1;

        arg_state = va_arg(argv, fmt_marker_generic);
        if (arg_state == GEN_END) {
            return FMT_ERR_NOT_ENOUGH_ARGS;
        }

        fmt_t display;
        switch (arg_state) {
            case GEN_END:{
#ifdef unreachable
                unreachable();
#endif
            } break;
            case GEN_INTERFACE:{
                display = va_arg(argv, fmt_t);
            } break;
            case GEN_INT:{
                int value = va_arg(argv, int);
                display = fmt.Int(&value);
            } break;
            case GEN_DOUBLE:{
                double value = va_arg(argv, double);
                display = fmt.Double(&value);
            } break;
            case GEN_CSTR:{
                const char *value = va_arg(argv, char *);
                display = fmt.CStr(value);
            } break;
            case GEN_ERR:{
                fmt_error value = va_arg(argv, fmt_error);
                display = fmt.errstr(value);
            } break;
            case GEN_BOOL: {
                bool value = va_arg(argv, int);
                display = fmt.Bool(value);
            } break;
#undef fmt_mixin
#define fmt_mixin(T, GEN, F) \
            case GEN: { \
                T value = va_arg(argv, T); \
                display = F(&value); \
            } break;
            GEN_MIXIN
#undef fmt_mixin
// set it back
#define fmt_mixin(T, GEN, F) T: GEN,
        }
        fmt_error err; 
        if ((err = display.fmt(display.ptr, stream))) return err;
    }

    return FMT_OK;
}

fmt_error INTERNAL(format)(FILE *stream, const char *fmt, ...) {
    va_list argv;
    va_start(argv, fmt);
    fmt_error err = INTERNAL(vformat)(stream, fmt, argv);
    va_end(argv);
    return err;
}

void INTERNAL(format_or_die)(FILE *stream, const char *fmt, const char *file, int line, ...) {
    va_list argv;
    va_start(argv, line);
    fmt_error err; 

    if ((err = INTERNAL(vformat)(stream, fmt, argv))) {
        report_error(file, line, err);
        exit(err);
    }

    va_end(argv);
}

fmt_error INTERNAL(int_fmt)(const void *ctx, FILE *stream) {
    const int *self = ctx;
    if (fprintf(stream, "%d", *self) < 0) return FMT_ERR_FPRINTF;
    return FMT_OK;
}
fmt_t INTERNAL(int_display)(int *self) {
    return (fmt_t){
        .ptr = self,
        .fmt = INTERNAL(int_fmt),
    };
}

fmt_error INTERNAL(double_fmt)(const void *ctx, FILE *stream) {
    const double *self = ctx;
    if (fprintf(stream, "%G", *self) < 0) return FMT_ERR_FPRINTF;
    return FMT_OK;
}
fmt_t INTERNAL(double_display)(double *self) {
    return (fmt_t){
        .ptr = self,
        .fmt = INTERNAL(double_fmt),
    };
}

fmt_error INTERNAL(cstr_fmt)(const void *ctx, FILE *stream) {
    if (fwrite(
            ctx,
            strlen(ctx),
            1,
            stream
        ) != 1) return FMT_ERR_FWRITE;
    return FMT_OK;
}
fmt_t INTERNAL(cstr_display)(const char *self) {
    return (fmt_t){
        .ptr = self,
        .fmt = INTERNAL(cstr_fmt),
    };
}

fmt_t INTERNAL(bool_display)(bool self) {
    return (fmt_t) {
        .ptr = self ? "true" : "false",
        .fmt = INTERNAL(cstr_fmt),
    };
}

fmt_t INTERNAL(fmt_error_display)(fmt_error self) {
    const char *str;
    switch (self) {
        case FMT_OK: {
            str = "OK";
        } break;
        case FMT_ERR_NO_OPENBRACKET: {
            str = "missing opening {";
        } break;
        case FMT_ERR_NO_CLOSEBRACKET: {
            str = "missing closing }";
        } break;
        case FMT_ERR_NOT_ENOUGH_ARGS: {
            str = "too few arguments";
        } break;
        case FMT_ERR_FPRINTF: {
            str = "fprintf error";
        } break;
        case FMT_ERR_FWRITE: {
            str = "fwrite error";
        } break;
    }
    return (fmt_t) {
        .ptr = str,
        .fmt = INTERNAL(cstr_fmt),
    };
}

const fmt_mod fmt = {
    ._format = INTERNAL(format),
    ._format_or_die = INTERNAL(format_or_die),
    .Int = INTERNAL(int_display),
    .Double = INTERNAL(double_display),
    .CStr = INTERNAL(cstr_display),
    .Bool = INTERNAL(bool_display),
    .errstr = INTERNAL(fmt_error_display),
};

#undef INTERNAL

#endif // !CFMT_IMPLEMENTATION
