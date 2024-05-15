#include <stdio.h>
#include <string.h>

#include "fmt.h"
#include "str.h"

static str_t from_cstr(char *cstr) {
    return (str_t) { .data = cstr, .len = strlen(cstr) };
}

static str_t slice(str_t self, size_t begin, size_t end) {
    return (str_t) {
        .data = self.data + begin,
        .len = end - begin,
    };
}

static fmt_error get_format_string(str_t self, char *fmt_buf) {
    if (sprintf(fmt_buf, "%%.%lus", self.len) < 0) {
        return FMT_ERR_FPRINTF;
    }

    return FMT_OK;
}

static fmt_error display(const void *ctx, FILE *stream) {
    const str_t *self = ctx;

    char slice_fmt[10] = {0};
    fmt_error err;
    if ((err = get_format_string(*self, slice_fmt))) return err;
    if (fprintf(stream, slice_fmt, self->data) < 0) return FMT_ERR_FPRINTF;
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
