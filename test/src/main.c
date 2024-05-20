#include <stddef.h>

#define GEN_MIXIN \
    fmt_mixin(str_t, GEN_STR, str.fmt)

#include "str.h"

#define CFMT_IMPLEMENTATION
#include "fmt.h"
#undef CFMT_IMPLEMENTATION

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    fmt.println("{}", foo);
    fmt.println("{} -- interface", str.fmt(&foo));
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
