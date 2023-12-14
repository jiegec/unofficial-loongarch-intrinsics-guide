#include "common.h"

int bz_d(v128 a) {
  int dst;
#include "vsetanyeqz_d.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(__lsx_bz_d(a));
  assert(__lsx_bz_d(a) == bz_d(a));

  a = __m128i{1, 0};
  assert(__lsx_bz_d(a));
  assert(__lsx_bz_d(a) == bz_d(a));

  a = __m128i{0, 1};
  assert(__lsx_bz_d(a));
  assert(__lsx_bz_d(a) == bz_d(a));

  a = __m128i{0x1111111111111111, 0x1111111111111111};
  assert(!__lsx_bz_d(a));
  assert(__lsx_bz_d(a) == bz_d(a));

  a = __m128i{0x0000000000000000, 0x1111111111111111};
  assert(__lsx_bz_d(a));
  assert(__lsx_bz_d(a) == bz_d(a));
}