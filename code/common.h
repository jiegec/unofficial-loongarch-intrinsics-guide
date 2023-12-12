#include <assert.h>
#include <lsxintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

union v128 {
  __m128i m128i;
  __m128 m128;
  __m128d m128d;
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

#define FUZZ2(func)                                                            \
  do {                                                                         \
    for (int i = 0; i < 64; i++) {                                             \
      v128 a, b;                                                               \
      PRINT(a);                                                                \
      PRINT(b);                                                                \
      PRINT(__lsx_##func(a, b));                                               \
      PRINT(func(a, b));                                                       \
      assert(func(a, b) == __lsx_##func(a, b));                                \
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
