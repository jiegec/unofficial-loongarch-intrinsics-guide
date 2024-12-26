#include "common.h"

v128 vftintrne_w_d(v128 a, v128 b) {
  v128 dst;
#include "vftintrne_w_d.h"
  return dst;
}

void test() {
  {
    __m128d a = {1.0, 2.0};
    __m128d b = {3.0, 4.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vftintrne_w_d(a, b));
    PRINT(vftintrne_w_d(a, b));
    assert(vftintrne_w_d(a, b) == __lsx_vftintrne_w_d(a, b));
  }
}
