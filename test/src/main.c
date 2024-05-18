#include "cursed_macros.h"
#include <stddef.h>

#define GEN_MIXIN \
    (str_t, GEN_STR), \
    (float, GEN_FLOAT)

#include "fmt.h"

#include "str.h"

#define GEN_MIXIN_IMPLEMENTATION \
    case GEN_STR: { \
        str_t value = va_arg(argv, str_t); \
        display = str.fmt(&value); \
    } break;

#define CFMT_IMPLEMENTATION
#include "fmt.h"


int main() {
    str_t foo = str.from_cstr("Hello, world!");
    fmt.println("{}", foo);
    str_t foo_slice = str.slice(foo, 3, 10);
    fmt.println("{}", foo_slice);

    fmt.println("{} | {}", foo, foo_slice);

    fmt.println("{}", 25);
    fmt.println("{}", 123.67);
    fmt.println("{{}}");
    fmt.println("{} != {}", true, false);

    fmt_error err;
    if ((err = fmt.print("{}\n"))) {
        FMT_REPORT(err);
    }

    if ((err = fmt.print("}\n"))) {
        FMT_REPORT(err);
    }

    if ((err = fmt.print("{\n"))) {
        FMT_REPORT(err);
    }
}
