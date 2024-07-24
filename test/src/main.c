#include <stdint.h>

#define FMT_INCLUDE_CONFIG_H
#include "fmt.h"

#include "str.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

DISPLAY(RGB)(const RGB *self, FILE *stream) {
    return fmt.format(
        stream,
        "RGB {{ r: {}, g: {}, b: {} }}",
        (int)self->r, (int)self->g, (int)self->b
    );
}

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    fmt.println("{} -- interface", str.fmt(&foo));
    fmt.println("{}", foo);
    str_t foo_slice = str.slice(foo, 3, 10);
    fmt.println("{}", foo_slice);

    fmt.println("{} | {}", foo, foo_slice);

    fmt.println("{}", 25);
    fmt.println("{}", 123.67);
    fmt.println("{{}}");
    fmt.println("{} != {}", true, false);

    fmt.println("{}", ((RGB){ .r = 5, .g = 255, .b = 8 }));

    fmt_error err;
    if ((err = fmt.print("{}\n"))) {
        fmt.report(err);
    }

    if ((err = fmt.print("}\n"))) {
        fmt.report(err);
    }

    if ((err = fmt.print("{\n"))) {
        fmt.report(err);
    }
}

#define CFMT_IMPLEMENTATION
#include "fmt.h"
