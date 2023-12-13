#include "common.h"

v128 vfsub_s(v128 a, v128 b) {
  v128 dst;
#include "vfsub_s.h"
  return dst;
}

void test() {
  {
    __m128 a = {1.0, 2.0, 3.0, 4.0};
    __m128 b = {5.0, 6.0, 7.0, 8.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vfsub_s(a, b));
    PRINT(vfsub_s(a, b));
    assert(vfsub_s(a, b) == __lsx_vfsub_s(a, b));
  }
}
