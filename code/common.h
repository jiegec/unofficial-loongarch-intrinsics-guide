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

#ifndef MACHINE_3C5000
#define MACHINE_3C5000 0
#endif

union v128 {
  __m128i m128i;
  __m128 m128;
  __m128d m128d;
  v4i32 __v4i32;
  v8i16 __v8i16;
  v16i8 __v16i8;

  u8 byte[16];
  u16 half[8];
  u32 word[4];
  u64 dword[2];
  __int128 qword[1];

  v128(__m128i other) { m128i = other; }
  v128() {
    for (int i = 0; i < 8; i++) {
      half[i] = rand();
    }
  }

  operator __m128i() { return m128i; }
  operator v4i32() { return __v4i32; }
  operator v8i16() { return __v8i16; }
  operator v16i8() { return __v16i8; }
  bool operator==(const v128 &other) const {
    return memcmp(byte, other.byte, 16) == 0;
  }
};

void test();

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
      PRINT(a);                                                                \
      PRINT(__lsx_##func(a __VA_OPT__(, ) __VA_ARGS__));                       \
      PRINT(func(a __VA_OPT__(, ) __VA_ARGS__));                               \
      assert(func(a __VA_OPT__(, ) __VA_ARGS__) ==                             \
             __lsx_##func(a __VA_OPT__(, ) __VA_ARGS__));                      \
    }                                                                          \
  } while (0);

#define FUZZ2(func, ...)                                                       \
  do {                                                                         \
    for (int i = 0; i < 64; i++) {                                             \
      v128 a, b;                                                               \
      PRINT(a);                                                                \
      PRINT(b);                                                                \
      PRINT(__lsx_##func(a, b __VA_OPT__(, ) __VA_ARGS__));                    \
      PRINT(func(a, b __VA_OPT__(, ) __VA_ARGS__));                            \
      assert(func(a, b __VA_OPT__(, ) __VA_ARGS__) ==                          \
             __lsx_##func(a, b __VA_OPT__(, ) __VA_ARGS__));                   \
    }                                                                          \
  } while (0);

#define FUZZ3(func)                                                            \
  do {                                                                         \
    for (int i = 0; i < 64; i++) {                                             \
      v128 a, b, c;                                                            \
      PRINT(a);                                                                \
      PRINT(b);                                                                \
      PRINT(c);                                                                \
      PRINT(__lsx_##func(a, b, c));                                            \
      PRINT(func(a, b, c));                                                    \
      assert(func(a, b, c) == __lsx_##func(a, b, c));                          \
    }                                                                          \
  } while (0);

int main(int argc, char *argv[]) {
  printf("Testing %s\n", argv[0]);
  test();
  return 0;
}
