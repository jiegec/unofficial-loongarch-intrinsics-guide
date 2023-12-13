#include "common.h"

v128 vftinth_l_s(v128 a) {
  v128 dst;
#include "vftinth_l_s.h"
  return dst;
}

void test() {
  {
    __m128 a = {1.0, 2.0, 3.0, 4.0};
    PRINT(a);
    PRINT(__lsx_vftinth_l_s(a));
    PRINT(vftinth_l_s(a));
    assert(vftinth_l_s(a) == __lsx_vftinth_l_s(a));
  }
}
