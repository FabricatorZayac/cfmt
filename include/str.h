#ifndef STR_H_FABRICATORZAYAC_
#define STR_H_FABRICATORZAYAC_

#include <stddef.h>

#include "fmt.h"

typedef struct {
    char *data;
    size_t len;
} str_t;

typedef struct {
    str_t (*from_cstr)(char *);
    str_t (*slice)(str_t, size_t, size_t);
    fmt_t (*fmt)(const str_t *);
} str_mt;

extern const str_mt str;

#endif // !STR_H_FABRICATORZAYAC_
