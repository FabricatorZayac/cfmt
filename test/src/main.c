#include <stddef.h>

#define CFMT_PRINT_MACROS
#include "fmt.h"
#include "str.h"

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    fmt.printu("{}\n", str.fmt(&foo));
    str_t foo_slice = str.slice(foo, 3, 10);
    fmt.printu("{}\n", str.fmt(&foo_slice));

    fmt.printu("{} | {}\n", str.fmt(&foo), str.fmt(&foo_slice));
    fmt.printu("{}\n", 25);
    fmt.printu("{{}}\n");

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
