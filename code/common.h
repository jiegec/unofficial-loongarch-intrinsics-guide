#include <algorithm>
#include <assert.h>
#include <lsxintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef __int128 s128;
typedef unsigned __int128 u128;

#ifndef MACHINE_3C5000
#define MACHINE_3C5000 0
#endif

template <typename T> u8 clo(T num) {
  for (int i = sizeof(T) * 8 - 1; i >= 0; i--) {
    if ((num & ((T)1 << i)) == 0) {
      return sizeof(T) * 8 - 1 - i;
    }
  }
  return sizeof(T) * 8;
}

template <typename T> u8 clz(T num) {
  for (int i = sizeof(T) * 8 - 1; i >= 0; i--) {
    if ((num & ((T)1 << i)) != 0) {
      return sizeof(T) * 8 - 1 - i;
    }
  }
  return sizeof(T) * 8;
}

using std::max;
using std::min;

union v128 {
  __m128i m128i;
  __m128 m128;
  __m128d m128d;
  v2i64 __v2i64;
  v2u64 __v2u64;
  v4i32 __v4i32;
  v4u32 __v4u32;
  v8i16 __v8i16;
  v8u16 __v8u16;
  v16i8 __v16i8;
  v16u8 __v16u8;

  u8 byte[16];
  u16 half[8];
  u32 word[4];
  u64 dword[2];
  u128 qword[1];

  float fp32[4];
  double fp64[2];

  v128(__m128i other) { m128i = other; }
  v128(__m128d other) { m128d = other; }
  v128(__m128 other) { m128 = other; }
  v128() {
    for (int i = 0; i < 8; i++) {
      half[i] = rand();
    }
  }

  operator __m128i() { return m128i; }
  operator __m128() { return m128; }
  operator __m128d() { return m128d; }
  // duplicate with __m128i
  // operator v2i64() { return __v2i64; }
  operator v2u64() { return __v2u64; }
  operator v4i32() { return __v4i32; }
  operator v4u32() { return __v4u32; }
  operator v8i16() { return __v8i16; }
  operator v8u16() { return __v8u16; }
  operator v16i8() { return __v16i8; }
  operator v16u8() { return __v16u8; }
  bool operator==(const v128 &other) const {
    return memcmp(byte, other.byte, 16) == 0;
  }
  bool operator!=(const v128 &other) const {
    return memcmp(byte, other.byte, 16) != 0;
  }
};

void test();

void print(const char *s, v128 num) {
  printf("v128 as __m128i %s: %016lx %016lx\n", s, num.dword[0], num.dword[1]);
  printf("v128 as __m128 %s: %f %f %f %f\n", s, num.fp32[0], num.fp32[1],
         num.fp32[2], num.fp32[3]);
  printf("v128 as __m128d %s: %lf %lf\n", s, num.fp64[0], num.fp64[1]);
}

void print(const char *s, __m128i num) {
  printf("__m128i %s: %016llx %016llx\n", s, num[0], num[1]);
}

void print(const char *s, __m128 num) {
  printf("__m128 %s: %f %f %f %f\n", s, num[0], num[1], num[2], num[3]);
}

void print(const char *s, __m128d num) {
  printf("__m128d %s: %lf %lf\n", s, num[0], num[1]);
}

#define PRINT(x) print(#x, x)

#define FUZZ1(func, ...)                                                       \
  do {                                                                         \
    for (int i = 0; i < 64; i++) {                                             \
      v128 a;                                                                  \
      if (func(a __VA_OPT__(, ) __VA_ARGS__) !=                                \
          __lsx_##func(a __VA_OPT__(, ) __VA_ARGS__)) {                        \
        PRINT(a);                                                              \
        PRINT(__lsx_##func(a __VA_OPT__(, ) __VA_ARGS__));                     \
        PRINT(func(a __VA_OPT__(, ) __VA_ARGS__));                             \
        assert(func(a __VA_OPT__(, ) __VA_ARGS__) ==                           \
               __lsx_##func(a __VA_OPT__(, ) __VA_ARGS__));                    \
      }                                                                        \
    }                                                                          \
  } while (0);

#define FUZZ2(func, ...)                                                       \
  do {                                                                         \
    for (int i = 0; i < 64; i++) {                                             \
      v128 a, b;                                                               \
      if (func(a, b __VA_OPT__(, ) __VA_ARGS__) !=                             \
          __lsx_##func(a, b __VA_OPT__(, ) __VA_ARGS__)) {                     \
        PRINT(a);                                                              \
        PRINT(b);                                                              \
        PRINT(__lsx_##func(a, b __VA_OPT__(, ) __VA_ARGS__));                  \
        PRINT(func(a, b __VA_OPT__(, ) __VA_ARGS__));                          \
        assert(func(a, b __VA_OPT__(, ) __VA_ARGS__) ==                        \
               __lsx_##func(a, b __VA_OPT__(, ) __VA_ARGS__));                 \
      }                                                                        \
    }                                                                          \
  } while (0);

#define FUZZ3(func)                                                            \
  do {                                                                         \
    for (int i = 0; i < 64; i++) {                                             \
      v128 a, b, c;                                                            \
      if (func(a, b, c) != __lsx_##func(a, b, c)) {                            \
        PRINT(a);                                                              \
        PRINT(b);                                                              \
        PRINT(c);                                                              \
        PRINT(__lsx_##func(a, b, c));                                          \
        PRINT(func(a, b, c));                                                  \
        assert(func(a, b, c) == __lsx_##func(a, b, c));                        \
      }                                                                        \
    }                                                                          \
  } while (0);

int main(int argc, char *argv[]) {
  printf("Testing %s\n", argv[0]);
  test();
  return 0;
}
