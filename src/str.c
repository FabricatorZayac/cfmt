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

DISPLAY(str_t)(const str_t *self, FILE *stream) {
    return fwrite(
        self->data,
        self->len,
        1,
        stream
    ) == 1
    ? FMT_OK
    : FMT_ERR_FWRITE;
}

const str_mt str = {
    .from_cstr = from_cstr,
    .slice = slice,
    .fmt = str_t_fmt,
};
