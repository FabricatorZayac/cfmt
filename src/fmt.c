#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"

#define SPECIFIER_MAX_LEN 7

static int count(const char *restrict fmt) {
    int ret = 0;
    for (size_t i = 0; i < strlen(fmt); i++) {
        if (fmt[i] == '{' && fmt[i + 1] == '}') {
            ret++;
        }
    }
    return ret;
}

fmt_error fmt_print(const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int argc = count(fmt);

    for (size_t i = 0; i < strlen(fmt); i++) {
        if (fmt[i] == '{' && fmt[++i] == '}') {
            Display_t arg = va_arg(args, Display_t);
            arg.vtable->fmt(arg.ptr, stdout);
        } else {
            fputc(fmt[i], stdout);
        }
    }

    va_end(args);

    return FMT_OK;
}

const fmt_mod fmt = {
    .print = fmt_print,
};
