#include "common.h"

int bz_b(v128 a) {
  int dst;
#include "vsetanyeqz_b.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(__lsx_bz_b(a));
  assert(__lsx_bz_b(a) == bz_b(a));

  a = __m128i{1, 0};
  assert(__lsx_bz_b(a));
  assert(__lsx_bz_b(a) == bz_b(a));

  a = __m128i{0, 1};
  assert(__lsx_bz_b(a));
  assert(__lsx_bz_b(a) == bz_b(a));

  a = __m128i{0x1111111111111111, 0x1111111111111111};
  assert(!__lsx_bz_b(a));
  assert(__lsx_bz_b(a) == bz_b(a));

  a = __m128i{0x1111111111111100, 0x1111111111111111};
  assert(__lsx_bz_b(a));
  assert(__lsx_bz_b(a) == bz_b(a));
}