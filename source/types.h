#ifndef FIS_TYPES_H
#define FIS_TYPES_H

#if defined(_WIN32) || defined(__linux__)
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;
#else
    #error please wait for support for more than linux and win32
#endif

#endif // FIS_TYPES_H
