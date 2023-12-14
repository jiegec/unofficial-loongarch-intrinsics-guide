#include "common.h"

int bz_w(v128 a) {
  int dst;
#include "vsetanyeqz_w.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(__lsx_bz_w(a));
  assert(__lsx_bz_w(a) == bz_w(a));

  a = __m128i{1, 0};
  assert(__lsx_bz_w(a));
  assert(__lsx_bz_w(a) == bz_w(a));

  a = __m128i{0, 1};
  assert(__lsx_bz_w(a));
  assert(__lsx_bz_w(a) == bz_w(a));

  a = __m128i{0x1111111111111111, 0x1111111111111111};
  assert(!__lsx_bz_w(a));
  assert(__lsx_bz_w(a) == bz_w(a));

  a = __m128i{0x1111111100000000, 0x1111111111111111};
  assert(__lsx_bz_w(a));
  assert(__lsx_bz_w(a) == bz_w(a));
}