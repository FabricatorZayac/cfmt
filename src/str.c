#include <stdio.h>
#include <string.h>

#include "fmt.h"
#include "str.h"

static str_t from_cstr(char *cstr) {
    return (str_t) { .data = cstr, .len = strlen(cstr) };
}

static str_t slice(str_t self, const range_t range) {
    return (str_t) {
        .data = self.data + range.begin,
        .len = range.end - range.begin,
    };
}

static fmt_error get_format_string(str_t self, char *fmt_buf) {
    if (sprintf(fmt_buf, "%%.%lus", self.len) < 0) {
        return FMT_ERR;
    }

    return FMT_OK;
}

static fmt_error display(const void *ctx, FILE *stream) {
    const str_t *self = ctx;

    char slice_fmt[10] = {0};
    if (get_format_string(*self, slice_fmt) == FMT_ERR) return FMT_ERR;
    if (fprintf(stream, slice_fmt, self->data) < 0) return FMT_ERR;
    return FMT_OK;
}

static fmt_t str_fmt(const str_t *self) {
    return (fmt_t) {
        .ptr = self,
        .fmt = display,
    };
}

const str_mt str = {
    .from_cstr = from_cstr,
    .slice = slice,
    .fmt = str_fmt,
};
