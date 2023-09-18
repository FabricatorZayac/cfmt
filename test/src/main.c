#include <stdio.h>
#include <string.h>

#include "fmt.h"
#include "str.h"

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    printf("%s\n", foo.data);

    str_t foo_slice = str.slice(foo, (range_t){3, 10});
    printf("%.7s\n", foo_slice.data);

    fmt.print("{} {}\n", str.display(&foo), str.display(&foo_slice));
}
