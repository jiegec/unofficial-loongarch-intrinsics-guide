#include "common.h"

void test() {
  __m128i a = {0, 0};
  assert(__lsx_bz_v(a));

  a = __m128i {1, 0};
  assert(!__lsx_bz_v(a));

  a = __m128i {0, 1};
  assert(!__lsx_bz_v(a));
}