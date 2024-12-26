#include "common.h"

v128 vftintrm_w_d(v128 a, v128 b) {
  v128 dst;
#include "vftintrm_w_d.h"
  return dst;
}

void test() {
  {
    __m128d a = {1.0, 2.0};
    __m128d b = {3.0, 4.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vftintrm_w_d(a, b));
    PRINT(vftintrm_w_d(a, b));
    assert(vftintrm_w_d(a, b) == __lsx_vftintrm_w_d(a, b));
  }
}
