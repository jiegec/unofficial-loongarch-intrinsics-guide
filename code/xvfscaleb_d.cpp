#include "common.h"

v256 xvfscaleb_d(v256 a, v256 b) {
  v256 dst;
#include "xvfscaleb_d.h"
  return dst;
}

void test() {
  {
    __m256d a = {1.0, 2.0, 3.0, 4.0};
    __m256i b = {3, 4, 1, 2};
    PRINT(a);
    PRINT(b);
    PRINT(__lasx_xvfscaleb_d(a, b));
    PRINT(xvfscaleb_d(a, b));
    assert(xvfscaleb_d(a, b) == __lasx_xvfscaleb_d(a, b));
  }
}
