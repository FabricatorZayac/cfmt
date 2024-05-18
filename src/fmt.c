#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fmt.h"

static fmt_error int_fmt(const void *ctx, FILE *stream);
static fmt_error double_fmt(const void *ctx, FILE *stream);

static fmt_error vformat(FILE *stream, const char *format, va_list argv) {
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
            fwrite(format + start_index, 1, end_index - start_index, stream);
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

        fmt_error err; 
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
        }
        if ((err = display.fmt(display.ptr, stream))) return err;
    }

    return FMT_OK;
}

static fmt_error _format(FILE *stream, const char *fmt, ...) {
    va_list argv;
    va_start(argv, fmt);
    fmt_error err = vformat(stream, fmt, argv);
    va_end(argv);
    return err;
}

static void _format_or_die(FILE *stream, const char *fmt, const char *file, int line, ...) {
    va_list argv;
    va_start(argv, line);
    fmt_error err; 

    if ((err = vformat(stream, fmt, argv))) {
        report_error(file, line, err);
        exit(err);
    }
    
    va_end(argv);
}

static fmt_error int_fmt(const void *ctx, FILE *stream) {
    const int *self = ctx;
    if (fprintf(stream, "%d", *self) < 0) return FMT_ERR_FPRINTF;
    return FMT_OK;
}
static fmt_t int_display(int *self) {
    return (fmt_t){
        .ptr = self,
        .fmt = int_fmt,
    };
}

static fmt_error double_fmt(const void *ctx, FILE *stream) {
    const float *self = ctx;
    if (fprintf(stream, "%G", *self) < 0) return FMT_ERR_FPRINTF;
    return FMT_OK;
}
static fmt_t double_display(double *self) {
    return (fmt_t){
        .ptr = self,
        .fmt = double_fmt,
    };
}

static fmt_error cstr_fmt(const void *ctx, FILE *stream) {
    const char *self = ctx;
    if (fprintf(stream, "%s", self) < 0) return FMT_ERR_FPRINTF; 
    return FMT_OK;
}
static fmt_t cstr_display(const char *self) {
    return (fmt_t){
        .ptr = self,
        .fmt = cstr_fmt,
    };
}

static fmt_t fmt_error_display(fmt_error self) {
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
        case FMT_ERR_UNSUPPORTED_TYPE: {
            str = "unsupported type";
        } break;
    }
    return (fmt_t) {
        .ptr = str,
        .fmt = cstr_fmt,
    };
}

const fmt_mod fmt = {
    ._format = _format,
    ._format_or_die = _format_or_die,
    .Int = int_display,
    .Double = double_display,
    .CStr = cstr_display,
    .errstr = fmt_error_display,
};
