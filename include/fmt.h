#ifndef FMT_H_FABRICATORZAYAC_
#define FMT_H_FABRICATORZAYAC_

typedef enum {
    FMT_OK,
    FMT_ERR,
} fmt_error;

typedef struct {
    fmt_error (*fmt)(const void *self, char *fmt_buf);
} Display_vtable;
typedef struct {
    const void *ptr;
    const Display_vtable *vtable;
} Display_t;

typedef struct {
    fmt_error (*println)(const char *restrict fmt, ...);
} fmt_mod;

extern const fmt_mod fmt;

#endif // !FMT_H_FABRICATORZAYAC_
