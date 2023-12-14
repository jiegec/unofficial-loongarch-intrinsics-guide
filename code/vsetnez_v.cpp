#include "common.h"

int bnz_v(v128 a) {
  int dst;
#include "vsetnez_v.h"
  return dst;
}

void test() {
  __m128i a = {0, 0};
  assert(!__lsx_bnz_v(a));
  assert(__lsx_bnz_v(a) == bnz_v(a));

  a = __m128i{1, 0};
  assert(__lsx_bnz_v(a));
  assert(__lsx_bnz_v(a) == bnz_v(a));

  a = __m128i{0, 1};
  assert(__lsx_bnz_v(a));
  assert(__lsx_bnz_v(a) == bnz_v(a));
}