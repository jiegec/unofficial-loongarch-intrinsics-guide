#include "common.h"

v128 vfadd_s(v128 a, v128 b) {
  v128 dst;
#include "vfadd_s.h"
  return dst;
}

void test() {
  {
    __m128 a = {1.0, 2.0, 3.0, 4.0};
    __m128 b = {5.0, 6.0, 7.0, 8.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vfadd_s(a, b));
    PRINT(vfadd_s(a, b));
    assert(vfadd_s(a, b) == __lsx_vfadd_s(a, b));
  }
}
