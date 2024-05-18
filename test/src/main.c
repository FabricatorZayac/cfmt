#include <stddef.h>

#include "fmt.h"
#include "str.h"

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    fmt.println("{}", str.fmt(&foo));
    str_t foo_slice = str.slice(foo, 3, 10);
    fmt.println("{}", str.fmt(&foo_slice));

    fmt.println("{} | {}", str.fmt(&foo), str.fmt(&foo_slice));
    fmt.println("{}", 25);
    fmt.println("{{}}");

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
