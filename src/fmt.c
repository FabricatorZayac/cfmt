#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fmt.h"
#undef print
#undef fprint

#define SPECIFIER_MAX_LEN 7

static fmt_error int_fmt(const void *ctx, FILE *stream);
static fmt_error double_fmt(const void *ctx, FILE *stream);

typedef enum {
    TOKEN_OBJ,
    TOKEN_INT,
    TOKEN_FLOAT,
} fmt_token;

typedef struct {
    size_t pos;
    int shift;
    fmt_token type;
} Token;

Token Token_new(size_t pos, fmt_token type) {
    Token ret = { .pos = pos, .type = type };
    switch (type) {
    case TOKEN_OBJ:
        ret.shift = 1;
        break;
    case TOKEN_INT:
    case TOKEN_FLOAT:
        ret.shift = 2;
        break;
    }
    return ret;
}

static int fmt_parse(const char *fmt, Token *tokens) {
    int n = 0;
    bool in_token = false;
    for (size_t i = 0; i < strlen(fmt); i++) {
        if (!in_token && fmt[i] == '{') in_token = true;
        if (in_token) {
            if (fmt[i + 1] == '}') {
                in_token = false;
                tokens[n] = Token_new(i, TOKEN_OBJ);
                ++n;
                i += 1;
            } else if (fmt[i + 1] == 'i' && fmt[i + 2] == '}') {
                in_token = false;
                tokens[n] = Token_new(i, TOKEN_INT);
                ++n;
                i += 2;
            } else {
                return -1; // error sentinel, invalid format string
            }
        }
    }
    return n;
}

static fmt_error vfprint(FILE *stream, const char *fmt, int argc, va_list argv) {
    Token tokens[argc];
    int n = fmt_parse(fmt, tokens);
    if (fmt_parse(fmt, tokens) != argc) return FMT_ERR;
    if (n == 0) {
        fputs(fmt, stream);
        return FMT_OK;
    }
    int token_idx = 0;
    for (size_t i = 0; i < strlen(fmt); i++) {
        if (tokens[token_idx].pos == i) {
            i += tokens[token_idx].shift;
            switch (tokens[token_idx].type) {
                case TOKEN_OBJ: {
                    fmt_t arg = va_arg(argv, fmt_t);
                    if (arg.fmt(arg.ptr, stream) == FMT_ERR) return FMT_ERR;
                } break;
                case TOKEN_INT: {
                    int arg = va_arg(argv, int);
                    if (int_fmt(&arg, stream)) return FMT_ERR;
                } break;
                case TOKEN_FLOAT: {
                    double arg = va_arg(argv, double);
                    if (double_fmt(&arg, stream)) return FMT_ERR;
                } break;
            }
            continue;
        }
        fputc(fmt[i], stream);
    }

    return FMT_OK;
}

static fmt_error fprint(FILE *stream, const char *fmt, int argc, ...) {
    va_list argv;
    va_start(argv, argc);
    fmt_error err = vfprint(stream, fmt, argc, argv);
    va_end(argv);
    return err;
}

static fmt_error print(const char *fmt, int argc, ...) {
    va_list argv;
    va_start(argv, argc);
    fmt_error err = vfprint(stdout, fmt, argc, argv);
    va_end(argv);
    return err;
}

static fmt_error int_fmt(const void *ctx, FILE *stream) {
    const int *self = ctx;
    if (fprintf(stream, "%d", *self) < 0) return FMT_ERR;
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
    if (fprintf(stream, "%G", *self) < 0) return FMT_ERR;
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
    if (fprintf(stream, "%s", self) < 0) return FMT_ERR; 
    return FMT_OK;
}
static fmt_t cstr_display(const char *self) {
    return (fmt_t){
        .ptr = self,
        .fmt = cstr_fmt,
    };
}

const fmt_mod fmt = {
    .print = print,
    .fprint = fprint,
    .Int = int_display,
    .Double = double_display,
    .CStr = cstr_display,
};
