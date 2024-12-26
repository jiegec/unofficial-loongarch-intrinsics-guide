#include "common.h"

v256 xvftintrz_w_d(v256 a, v256 b) {
  v256 dst;
#include "xvftintrz_w_d.h"
  return dst;
}

void test() {
  {
    __m256d a = {1.0, 2.0, 3.0, 4.0};
    __m256d b = {5.0, 6.0, 7.0, 8.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lasx_xvftintrz_w_d(a, b));
    PRINT(xvftintrz_w_d(a, b));
    assert(xvftintrz_w_d(a, b) == __lasx_xvftintrz_w_d(a, b));
  }
}
