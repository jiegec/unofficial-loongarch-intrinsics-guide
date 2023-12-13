#include "common.h"

v128 vfadd_d(v128 a, v128 b) {
  v128 dst;
#include "vfadd_d.h"
  return dst;
}

void test() {
  {
    __m128d a = {1.0, 2.0};
    __m128d b = {5.0, 6.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vfadd_d(a, b));
    PRINT(vfadd_d(a, b));
    assert(vfadd_d(a, b) == __lsx_vfadd_d(a, b));
  }
}
