#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef unsigned long dword;

typedef float  f32;
typedef double f64;

typedef bool b8;

typedef struct string_view_s {
    const char* data;
    u64 len;
} string_view;

typedef struct color_s {
    union {
        struct {
            u8 a, r, g, b;
        };
        i32 hex;
    };
} color_t;

i32 color_as_hex(color_t color);
color_t hex_as_color(i32 hex);

b8 sv_equals(string_view a, string_view b);
