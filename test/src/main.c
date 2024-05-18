#include "cursed_macros.h"
#include <stddef.h>

// #define GEN_MIXIN \
//     (str_t, GEN_STR), \
//     (float, GEN_FLOAT),

#include "fmt.h"

#include "str.h"

#define GEN_MIXIN_IMPL(T, GEN, F) \
    case GEN: { \
        T value = va_arg(argv, T); \
        display = F(&value); \
    } break;

#define GEN_MIXIN_IMPLEMENTATION \
    GEN_MIXIN_IMPL(str_t, GEN_STR, str.fmt)

#define CFMT_IMPLEMENTATION
#include "fmt.h"

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    // fmt.println("{}", foo);
    fmt.println("{}", str.fmt(&foo));
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
