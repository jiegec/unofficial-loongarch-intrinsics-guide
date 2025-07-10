#include "common.h"

v128 vfscaleb_s(v128 a, v128 b) {
  v128 dst;
#include "vfscaleb_s.h"
  return dst;
}

void test() {
  {
    __m128 a = {1.0, 2.0, 3.0, 4.0};
    __m128i b = {((uint64_t)5 << 32) | 6, ((uint64_t)7 << 32) | 8};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vfscaleb_s(a, b));
    PRINT(vfscaleb_s(a, b));
    assert(vfscaleb_s(a, b) == __lsx_vfscaleb_s(a, b));
  }
}
