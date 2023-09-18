#include <stdio.h>
#include <string.h>

#include "fmt.h"
#include "str.h"

str_t str_t_from_cstr(char *cstr) {
    return (str_t) { .data = cstr, .len = strlen(cstr) };
}

str_t str_t_slice(str_t self, const range_t range) {
    return (str_t) {
        .data = self.data + range.begin,
        .len = range.end - range.begin,
    };
}

fmt_error str_t_display_fmt(const void *self, char *fmt_buf) {
    const str_t *this = self;

    fmt_buf[0] = '%';
    fmt_buf[1] = '.';
    if (sprintf(fmt_buf + 2, "%lus", this->len) < 0) {
        return FMT_ERR;
    }

    return FMT_OK;
}
const static Display_vtable str_t_Display_vtable = {
    .fmt = str_t_display_fmt,
};
Display_t str_t_display(const str_t *self) {
    return (Display_t) {
        .ptr = self,
        .vtable = &str_t_Display_vtable,
    };
}

const str_mt str = {
    .from_cstr = str_t_from_cstr,
    .slice = str_t_slice,
};
