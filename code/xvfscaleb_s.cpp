#include "common.h"

v256 xvfscaleb_s(v256 a, v256 b) {
  v256 dst;
#include "xvfscaleb_s.h"
  return dst;
}

void test() {
  {
    __m256 a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    __m256i b = {((uint64_t)5 << 32) | 6, ((uint64_t)7 << 32) | 8,
                 ((uint64_t)1 << 32) | 2, ((uint64_t)3 << 32) | 4};
    PRINT(a);
    PRINT(b);
    PRINT(__lasx_xvfscaleb_s(a, b));
    PRINT(xvfscaleb_s(a, b));
    assert(xvfscaleb_s(a, b) == __lasx_xvfscaleb_s(a, b));
  }
}
