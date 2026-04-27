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

typedef float  f32;
typedef double f64;

typedef bool b8;

typedef struct {
    const char* data;
    i32 len;          
    u64 size;        
} string_view;

#define SV(s) (string_view){ .data = s, .len = (i32)sizeof(s) - 1, .size = (i32)sizeof(s) }

b8 sv_equals(string_view a, string_view b);
