#include "common.h"

int xbz_h(v256 a) {
  int dst;
#include "xvsetanyeqz_h.h"
  return dst;
}

void test() {
  __m256i a = {0, 0};
  assert(__lasx_xbz_h(a));
  assert(__lasx_xbz_h(a) == xbz_h(a));

  a = __m256i{1, 0};
  assert(__lasx_xbz_h(a));
  assert(__lasx_xbz_h(a) == xbz_h(a));

  a = __m256i{0, 1};
  assert(__lasx_xbz_h(a));
  assert(__lasx_xbz_h(a) == xbz_h(a));

  a = __m256i{0x1111111111111111, 0x1111111111111111, 0x1111111111111111, 0x1111111111111111};
  assert(!__lasx_xbz_h(a));
  assert(__lasx_xbz_h(a) == xbz_h(a));

  a = __m256i{0x1111111111110000, 0x1111111111111111, 0x1111111111111111, 0x1111111111111111};
  assert(__lasx_xbz_h(a));
  assert(__lasx_xbz_h(a) == xbz_h(a));
}