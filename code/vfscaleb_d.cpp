#include "common.h"

v128 vfscaleb_d(v128 a, v128 b) {
  v128 dst;
#include "vfscaleb_d.h"
  return dst;
}

void test() {
  {
    __m128d a = {1.0, 2.0};
    __m128i b = {3, 4};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vfscaleb_d(a, b));
    PRINT(vfscaleb_d(a, b));
    assert(vfscaleb_d(a, b) == __lsx_vfscaleb_d(a, b));
  }
}
