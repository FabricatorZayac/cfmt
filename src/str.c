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

static fmt_error get_format_string(const void *ctx, char *fmt_buf) {
    const str_t *self = ctx;

    if (sprintf(fmt_buf, "%%.%lus", self->len) < 0) {
        return FMT_ERR;
    }

    return FMT_OK;
}

static void _fmt(const void *ctx, FILE *stream) {
    const str_t *self = ctx;

    char slice_fmt[10] = {0};
    Display_t display = str.display(self);
    get_format_string(display.ptr, slice_fmt);
    fprintf(stream, slice_fmt, self->data);
}

const static Display_vtable str_t_Display_vtable = {
    .fmt = _fmt,
};

static Display_t display(const str_t *self) {
    return (Display_t) {
        .ptr = self,
        .vtable = &str_t_Display_vtable,
    };
}

const str_mt str = {
    .from_cstr = from_cstr,
    .slice = slice,
    .display = display,
};
