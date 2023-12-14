#include "common.h"

int xbnz_w(v256 a) {
  int dst;
#include "xvsetallnez_w.h"
  return dst;
}

void test() {
  __m256i a = {0, 0};
  assert(!__lasx_xbnz_w(a));
  assert(__lasx_xbnz_w(a) == xbnz_w(a));

  a = __m256i{1, 0};
  assert(!__lasx_xbnz_w(a));
  assert(__lasx_xbnz_w(a) == xbnz_w(a));

  a = __m256i{0, 1};
  assert(!__lasx_xbnz_w(a));
  assert(__lasx_xbnz_w(a) == xbnz_w(a));

  a = __m256i{0x1111111111111111, 0x1111111111111111, 0x1111111111111111,
              0x1111111111111111};
  assert(__lasx_xbnz_w(a));
  assert(__lasx_xbnz_w(a) == xbnz_w(a));

  a = __m256i{0x1111111100000000, 0x1111111111111111, 0x1111111111111111,
              0x1111111111111111};
  assert(!__lasx_xbnz_w(a));
  assert(__lasx_xbnz_w(a) == xbnz_w(a));
}