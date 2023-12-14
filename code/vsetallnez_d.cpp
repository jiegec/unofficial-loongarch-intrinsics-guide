#include "common.h"

int bnz_d(v128 a) {
  int dst;
#include "vsetallnez_d.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(!__lsx_bnz_d(a));
  assert(__lsx_bnz_d(a) == bnz_d(a));

  a = __m128i{1, 0};
  assert(!__lsx_bnz_d(a));
  assert(__lsx_bnz_d(a) == bnz_d(a));

  a = __m128i{0, 1};
  assert(!__lsx_bnz_d(a));
  assert(__lsx_bnz_d(a) == bnz_d(a));

  a = __m128i{0x1111111111111111, 0x1111111111111111};
  assert(__lsx_bnz_d(a));
  assert(__lsx_bnz_d(a) == bnz_d(a));

  a = __m128i{0x0000000000000000, 0x1111111111111111};
  assert(!__lsx_bnz_d(a));
  assert(__lsx_bnz_d(a) == bnz_d(a));
}