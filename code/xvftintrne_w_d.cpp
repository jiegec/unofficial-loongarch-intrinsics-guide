#include "common.h"

v256 xvftintrne_w_d(v256 a, v256 b) {
  v256 dst;
#include "xvftintrne_w_d.h"
  return dst;
}

void test() {
  {
    __m256d a = {1.0, 2.0, 3.0, 4.0};
    __m256d b = {5.0, 6.0, 7.0, 8.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lasx_xvftintrne_w_d(a, b));
    PRINT(xvftintrne_w_d(a, b));
    assert(xvftintrne_w_d(a, b) == __lasx_xvftintrne_w_d(a, b));
  }
}
