#include "common.h"

int bnz_b(v128 a) {
  int dst;
#include "vsetallnez_b.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(!__lsx_bnz_b(a));
  assert(__lsx_bnz_b(a) == bnz_b(a));

  a = __m128i{1, 0};
  assert(!__lsx_bnz_b(a));
  assert(__lsx_bnz_b(a) == bnz_b(a));

  a = __m128i{0, 1};
  assert(!__lsx_bnz_b(a));
  assert(__lsx_bnz_b(a) == bnz_b(a));

  a = __m128i{0x1111111111111111, 0x1111111111111111};
  assert(__lsx_bnz_b(a));
  assert(__lsx_bnz_b(a) == bnz_b(a));

  a = __m128i{0x1111111111111100, 0x1111111111111111};
  assert(!__lsx_bnz_b(a));
  assert(__lsx_bnz_b(a) == bnz_b(a));
}