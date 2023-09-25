#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "typeinfo.h"
#include "cursed_macros.h"
#include "fmt.h"
#include "str.h"

typedef struct {} Allocator_t;

typedef struct {
    typeinfo_t Self;
    size_t size;
    size_t cap;
    void *data;
    Allocator_t allocator;
} ArrayList_t;

typeinfo_t ArrayList(const typeinfo_t T) {
    return (typeinfo_t) {
        .size = sizeof(ArrayList_t),
    };
}

int main() {
    str_t foo = str.from_cstr("Hello, world!");
    fmt.print("{}\n", str.fmt(&foo));
    str_t foo_slice = str.slice(foo, (range_t){3, 10});
    fmt.print("{}\n", str.fmt(&foo_slice));

    fmt_error err = fmt.print("{} | {}\n",
                              str.fmt(&foo),
                              str.fmt(&foo_slice));
    if (err != FMT_OK) {
        fmt.print("error: "__FILE__":{}\n", fmt.Int(&(int){__LINE__}));
    }

    err = fmt.print("{} | {} | {}\n",
                    str.fmt(&foo),
                    str.fmt(&foo_slice));
    if (err != FMT_OK) {
        fmt.print("error: "__FILE__":{}\n", fmt.Int(&(int){__LINE__}));
    }

    fmt.print("{i}\n", 5);
    fmt.print("asd\n");
    fmt.print("{}\n", fmt.Double(&(double){25.1234}));

    err = fmt.print("{}\n");
    if (err != FMT_OK) {
        fmt.print("error: "__FILE__":{}\n", fmt.Int(&(int){__LINE__}));
    }
}
