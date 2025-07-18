#include <algorithm>
#include <assert.h>
#include <lasxintrin.h>
#include <limits>
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
typedef float f32;
typedef double f64;

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

template <typename T> u8 popcount(T num) {
  u8 res = 0;
  for (int i = sizeof(T) * 8 - 1; i >= 0; i--) {
    if ((num & ((T)1 << i)) != 0) {
      res++;
    }
  }
  return res;
}

// saturating add/sub
// Adapted from:
// https://codereview.stackexchange.com/questions/179172/c17-saturating-integer-arithmetic-type-library
template <typename T> T sadd(T a, T b) {
  T result;
  if (b < 0) {
    return __builtin_add_overflow(a, b, &result) ? std::numeric_limits<T>::min()
                                                 : result;
  } else {
    return __builtin_add_overflow(a, b, &result) ? std::numeric_limits<T>::max()
                                                 : result;
  }
}

template <typename T> T ssub(T a, T b) {
  T result;
  if (b < 0) {
    return __builtin_sub_overflow(a, b, &result) ? std::numeric_limits<T>::max()
                                                 : result;
  } else {
    return __builtin_sub_overflow(a, b, &result) ? std::numeric_limits<T>::min()
                                                 : result;
  }
}

using std::clamp;
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

union v256 {
  __m256i m256i;
  __m256 m256;
  __m256d m256d;
  v4i64 __v4i64;
  v4u64 __v4u64;
  v8i32 __v8i32;
  v8u32 __v8u32;
  v16i16 __v16i16;
  v16u16 __v16u16;
  v32i8 __v32i8;
  v32u8 __v32u8;

  u8 byte[32];
  u16 half[16];
  u32 word[8];
  u64 dword[4];
  u128 qword[2];

  float fp32[8];
  double fp64[4];

  v256(__m256i other) { m256i = other; }
  v256(__m256d other) { m256d = other; }
  v256(__m256 other) { m256 = other; }
  v256() {
    for (int i = 0; i < 16; i++) {
      half[i] = rand();
    }
  }

  operator __m256i() { return m256i; }
  operator __m256() { return m256; }
  operator __m256d() { return m256d; }
  // duplicate with __m256i
  // operator v4i64() { return __v4i64; }
  operator v4u64() { return __v4u64; }
  operator v8i32() { return __v8i32; }
  operator v8u32() { return __v8u32; }
  operator v16i16() { return __v16i16; }
  operator v16u16() { return __v16u16; }
  operator v32i8() { return __v32i8; }
  operator v32u8() { return __v32u8; }
  bool operator==(const v256 &other) const {
    return memcmp(byte, other.byte, 32) == 0;
  }
  bool operator!=(const v256 &other) const {
    return memcmp(byte, other.byte, 32) != 0;
  }
};

void test();

void print(const char *s, v128 num) {
  printf("v128 as __m128i %s: 0x%016lx 0x%016lx\n", s, num.dword[0],
         num.dword[1]);
  printf("v128 as __m128 %s: %f %f %f %f\n", s, num.fp32[0], num.fp32[1],
         num.fp32[2], num.fp32[3]);
  printf("v128 as __m128d %s: %lf %lf\n", s, num.fp64[0], num.fp64[1]);
}

void print(const char *s, __m128i num) {
  printf("__m128i %s: 0x%016llx 0x%016llx\n", s, num[0], num[1]);
}

void print(const char *s, __m128 num) {
  printf("__m128 %s: %f %f %f %f\n", s, num[0], num[1], num[2], num[3]);
}

void print(const char *s, __m128d num) {
  printf("__m128d %s: %lf %lf\n", s, num[0], num[1]);
}

void print(const char *s, v256 num) {
  printf("v256 as __m256i %s: 0x%016lx 0x%016lx 0x%016lx 0x%016lx\n", s,
         num.dword[0], num.dword[1], num.dword[2], num.dword[3]);
  printf("v256 as __m256 %s: %f %f %f %f %f %f %f %f\n", s, num.fp32[0],
         num.fp32[1], num.fp32[2], num.fp32[3], num.fp32[4], num.fp32[5],
         num.fp32[6], num.fp32[7]);
  printf("v256 as __m256d %s: %lf %lf %lf %lf\n", s, num.fp64[0], num.fp64[1],
         num.fp64[2], num.fp64[3]);
}

void print(const char *s, __m256i num) {
  printf("__m256i %s: 0x%016llx 0x%016llx 0x%016llx 0x%016llx\n", s, num[0],
         num[1], num[2], num[3]);
}

void print(const char *s, __m256 num) {
  printf("__m256 %s: %f %f %f %f %f %f %f %f\n", s, num[0], num[1], num[2],
         num[3], num[4], num[5], num[6], num[7]);
}

void print(const char *s, __m256d num) {
  printf("__m256d %s: %lf %lf %lf %lf\n", s, num[0], num[1], num[2], num[3]);
}

void print(const char *s, int num) { printf("int %s: %d\n", s, num); }

#define PRINT(x) print(#x, x)

#define FUZZ_N 128

#define FUZZ0(func, ...)                                                       \
  do {                                                                         \
    for (int i = 0; i < FUZZ_N; i++) {                                         \
      if (func(__VA_ARGS__) != __lsx_##func(__VA_ARGS__)) {                    \
        PRINT(__lsx_##func(__VA_ARGS__));                                      \
        PRINT(func(__VA_ARGS__));                                              \
        assert(func(__VA_ARGS__) == __lsx_##func(__VA_ARGS__));                \
      }                                                                        \
    }                                                                          \
  } while (0);

#define FUZZ1(func, ...)                                                       \
  do {                                                                         \
    for (int i = 0; i < FUZZ_N; i++) {                                         \
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
    for (int i = 0; i < FUZZ_N; i++) {                                         \
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
    for (int i = 0; i < FUZZ_N; i++) {                                         \
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

#define XFUZZ0(func, ...)                                                      \
  do {                                                                         \
    for (int i = 0; i < FUZZ_N; i++) {                                         \
      if (func(__VA_ARGS__) != __lasx_##func(__VA_ARGS__)) {                   \
        PRINT(__lasx_##func(__VA_ARGS__));                                     \
        PRINT(func(__VA_ARGS__));                                              \
        assert(func(__VA_ARGS__) == __lasx_##func(__VA_ARGS__));               \
      }                                                                        \
    }                                                                          \
  } while (0);

#define XFUZZ1(func, ...)                                                      \
  do {                                                                         \
    for (int i = 0; i < FUZZ_N; i++) {                                         \
      v256 a;                                                                  \
      if (func(a __VA_OPT__(, ) __VA_ARGS__) !=                                \
          __lasx_##func(a __VA_OPT__(, ) __VA_ARGS__)) {                       \
        PRINT(a);                                                              \
        PRINT(__lasx_##func(a __VA_OPT__(, ) __VA_ARGS__));                    \
        PRINT(func(a __VA_OPT__(, ) __VA_ARGS__));                             \
        assert(func(a __VA_OPT__(, ) __VA_ARGS__) ==                           \
               __lasx_##func(a __VA_OPT__(, ) __VA_ARGS__));                   \
      }                                                                        \
    }                                                                          \
  } while (0);

#define XFUZZ2(func, ...)                                                      \
  do {                                                                         \
    for (int i = 0; i < FUZZ_N; i++) {                                         \
      v256 a, b;                                                               \
      if (func(a, b __VA_OPT__(, ) __VA_ARGS__) !=                             \
          __lasx_##func(a, b __VA_OPT__(, ) __VA_ARGS__)) {                    \
        PRINT(a);                                                              \
        PRINT(b);                                                              \
        PRINT(__lasx_##func(a, b __VA_OPT__(, ) __VA_ARGS__));                 \
        PRINT(func(a, b __VA_OPT__(, ) __VA_ARGS__));                          \
        assert(func(a, b __VA_OPT__(, ) __VA_ARGS__) ==                        \
               __lasx_##func(a, b __VA_OPT__(, ) __VA_ARGS__));                \
      }                                                                        \
    }                                                                          \
  } while (0);

#define XFUZZ3(func)                                                           \
  do {                                                                         \
    for (int i = 0; i < FUZZ_N; i++) {                                         \
      v256 a, b, c;                                                            \
      if (func(a, b, c) != __lasx_##func(a, b, c)) {                           \
        PRINT(a);                                                              \
        PRINT(b);                                                              \
        PRINT(c);                                                              \
        PRINT(__lasx_##func(a, b, c));                                         \
        PRINT(func(a, b, c));                                                  \
        assert(func(a, b, c) == __lasx_##func(a, b, c));                       \
      }                                                                        \
    }                                                                          \
  } while (0);

int main(int argc, char *argv[]) {
  printf("Testing %s\n", argv[0]);
  test();
  return 0;
}

// add undocumented intrinsics
__m128 __lsx_vfscaleb_s(__m128 a, __m128i b) {
  register __m128 a_reg asm("vr1") = a;
  register __m128i b_reg asm("vr2") = b;
  register __m128 result asm("vr0");
  // vfscaleb.s vr0, vr1, vr2
  asm volatile(".word 0x71448000 + 0 + (1 << 5) + (2 << 10)"
               : "=f"(result)
               : "f"(a_reg), "f"(b_reg)
               : "memory");
  return result;
}

__m128d __lsx_vfscaleb_d(__m128d a, __m128i b) {
  register __m128d a_reg asm("vr1") = a;
  register __m128i b_reg asm("vr2") = b;
  register __m128d result asm("vr0");
  // vfscaleb.d vr0, vr1, vr2
  asm volatile(".word 0x71450000 + 0 + (1 << 5) + (2 << 10)"
               : "=f"(result)
               : "f"(a_reg), "f"(b_reg)
               : "memory");
  return result;
}

__m256 __lasx_xvfscaleb_s(__m256 a, __m256i b) {
  register __m256 a_reg asm("xr1") = a;
  register __m256i b_reg asm("xr2") = b;
  register __m256 result asm("xr0");
  // xvfscaleb.s xr0, xr1, xr2
  asm volatile(".word 0x75448000 + 0 + (1 << 5) + (2 << 10)"
               : "=f"(result)
               : "f"(a_reg), "f"(b_reg)
               : "memory");
  return result;
}

__m256d __lasx_xvfscaleb_d(__m256d a, __m256i b) {
  register __m256d a_reg asm("xr1") = a;
  register __m256i b_reg asm("xr2") = b;
  register __m256d result asm("xr0");
  // xvfscaleb.d xr0, xr1, xr2
  asm volatile(".word 0x75450000 + 0 + (1 << 5) + (2 << 10)"
               : "=f"(result)
               : "f"(a_reg), "f"(b_reg)
               : "memory");
  return result;
}

// vmepatmsk.v vr0, mode, imm
#define __lsx_vmepatmsk_v(mode, imm)                                           \
  ([=]() {                                                                     \
    register __m128i result asm("vr0");                                        \
    asm volatile(".word 0x729b8000 + 0 + (" #mode " << 5) + (" #imm " << 10)"  \
                 : "=f"(result)                                                \
                 :                                                             \
                 : "memory");                                                  \
    return result;                                                             \
  })()

// xvmepatmsk.v xr0, mode, imm
#define __lasx_xvmepatmsk_v(mode, imm)                                         \
  ([=]() {                                                                     \
    register __m256i result asm("xr0");                                        \
    asm volatile(".word 0x769b8000 + 0 + (" #mode " << 5) + (" #imm " << 10)"  \
                 : "=f"(result)                                                \
                 :                                                             \
                 : "memory");                                                  \
    return result;                                                             \
  })()
