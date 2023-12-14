#include "common.h"

void test() {
  __m128i a = {0, 0};
  assert(!__lsx_bnz_w(a));

  a = __m128i {1, 0};
  assert(!__lsx_bnz_w(a));

  a = __m128i {0, 1};
  assert(!__lsx_bnz_w(a));

  a = __m128i {0x1111111111111111, 0x1111111111111111};
  assert(__lsx_bnz_w(a));

  a = __m128i {0x1111111100000000, 0x1111111111111111};
  assert(!__lsx_bnz_w(a));
}