#include "common.h"

int xbnz_b(v256 a) {
  int dst;
#include "xvsetallnez_b.h"
  return dst;
}

void test() {
  __m256i a = {0, 0};
  assert(!__lasx_xbnz_b(a));
  assert(__lasx_xbnz_b(a) == xbnz_b(a));

  a = __m256i{1, 0};
  assert(!__lasx_xbnz_b(a));
  assert(__lasx_xbnz_b(a) == xbnz_b(a));

  a = __m256i{0, 1};
  assert(!__lasx_xbnz_b(a));
  assert(__lasx_xbnz_b(a) == xbnz_b(a));

  a = __m256i{0x1111111111111111, 0x1111111111111111, 0x1111111111111111,
              0x1111111111111111};
  assert(__lasx_xbnz_b(a));
  assert(__lasx_xbnz_b(a) == xbnz_b(a));

  a = __m256i{0x1111111111111100, 0x1111111111111111, 0x1111111111111111,
              0x1111111111111111};
  assert(!__lasx_xbnz_b(a));
  assert(__lasx_xbnz_b(a) == xbnz_b(a));
}