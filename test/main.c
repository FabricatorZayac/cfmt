#include <stdio.h>

#include "fmt.h"
#include "str.h"

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    printf("%s\n", foo.data);
    str_t foo_slice = str.slice(foo, (range_t){3, 10});
    printf("%.7s\n", foo_slice.data);

    char slice_fmt[10] = {0};
    fmt.println("{}\n", str.display(&foo_slice));
}
