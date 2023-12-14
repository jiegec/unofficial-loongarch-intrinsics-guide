#include "common.h"

int bnz_w(v128 a) {
  int dst;
#include "vsetallnez_w.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(!__lsx_bnz_w(a));
  assert(__lsx_bnz_w(a) == bnz_w(a));

  a = __m128i{1, 0};
  assert(!__lsx_bnz_w(a));
  assert(__lsx_bnz_w(a) == bnz_w(a));

  a = __m128i{0, 1};
  assert(!__lsx_bnz_w(a));
  assert(__lsx_bnz_w(a) == bnz_w(a));

  a = __m128i{0x1111111111111111, 0x1111111111111111};
  assert(__lsx_bnz_w(a));
  assert(__lsx_bnz_w(a) == bnz_w(a));

  a = __m128i{0x1111111111111100, 0x1111111111111111};
  assert(!__lsx_bnz_w(a));
  assert(__lsx_bnz_w(a) == bnz_w(a));
}