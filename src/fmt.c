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

fmt_error fmt_println(const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int argc = count(fmt);

    char *fmt_buf = calloc(strlen(fmt) + argc * SPECIFIER_MAX_LEN, sizeof(char));

    for (size_t i = 0, j = 0; i < strlen(fmt); i++, j++) {
        if (fmt[i] == '{' && fmt[++i] == '}') {
            Display_t arg = va_arg(args, Display_t);
            char sp[SPECIFIER_MAX_LEN] = {0};
            arg.vtable->fmt(arg.ptr, sp);
            strcat(fmt_buf, sp);
            j += strlen(sp);
        } else {
            fmt_buf[j] = fmt[i];
        }
    }

    vprintf(fmt_buf, args); 

    va_end(args);
    free(fmt_buf);

    return FMT_OK;
}

const fmt_mod fmt = {
    .println = fmt_println,
};
