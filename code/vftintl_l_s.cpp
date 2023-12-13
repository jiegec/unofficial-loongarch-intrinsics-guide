#include "common.h"

v128 vftintl_l_s(v128 a) {
  v128 dst;
#include "vftintl_l_s.h"
  return dst;
}

void test() {
  {
    __m128 a = {1.0, 2.0, 3.0, 4.0};
    PRINT(a);
    PRINT(__lsx_vftintl_l_s(a));
    PRINT(vftintl_l_s(a));
    assert(vftintl_l_s(a) == __lsx_vftintl_l_s(a));
  }
}
