#include "common.h"

int bz_h(v128 a) {
  int dst;
#include "vsetanyeqz_h.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(__lsx_bz_h(a));
  assert(__lsx_bz_h(a) == bz_h(a));

  a = __m128i{1, 0};
  assert(__lsx_bz_h(a));
  assert(__lsx_bz_h(a) == bz_h(a));

  a = __m128i{0, 1};
  assert(__lsx_bz_h(a));
  assert(__lsx_bz_h(a) == bz_h(a));

  a = __m128i{0x1111111111111111, 0x1111111111111111};
  assert(!__lsx_bz_h(a));
  assert(__lsx_bz_h(a) == bz_h(a));

  a = __m128i{0x1111111111110000, 0x1111111111111111};
  assert(__lsx_bz_h(a));
  assert(__lsx_bz_h(a) == bz_h(a));
}