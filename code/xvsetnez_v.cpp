#include "common.h"

int xbnz_v(v256 a) {
  int dst;
#include "xvsetnez_v.h"
  return dst;
}

void test() {
  __m256i a = {0, 0};
  assert(!__lasx_xbnz_v(a));
  assert(__lasx_xbnz_v(a) == xbnz_v(a));

  a = __m256i{1, 0};
  assert(__lasx_xbnz_v(a));
  assert(__lasx_xbnz_v(a) == xbnz_v(a));

  a = __m256i{0, 1};
  assert(__lasx_xbnz_v(a));
  assert(__lasx_xbnz_v(a) == xbnz_v(a));
}