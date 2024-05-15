#include <stddef.h>

#define CFMT_PRINT_MACROS
#include "fmt.h"
#include "str.h"

void report_error(const char *file, int line, fmt_error err) {
    (void)fmt.print("{}:{}: error: {}\n", file, line, err);
}
#define REPORT(ERR) report_error(__FILE__, __LINE__, ERR)

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    (void)fmt.print("{}\n", str.fmt(&foo));
    str_t foo_slice = str.slice(foo, 3, 10);
    (void)fmt.print("{}\n", str.fmt(&foo_slice));

    fmt_error err;
    if ((err = fmt.print(
        "{} | {}\n",
        str.fmt(&foo),
        str.fmt(&foo_slice)
    ))) {
        REPORT(err);
    }

    (void)fmt.print("{}\n", 25);
    (void)fmt.print("{{}}\n");

    if ((err = fmt.print("{}\n"))) {
        REPORT(err);
    }

    if ((err = fmt.print("}\n"))) {
        REPORT(err);
    }

    if ((err = fmt.print("{\n"))) {
        REPORT(err);
    }
}
