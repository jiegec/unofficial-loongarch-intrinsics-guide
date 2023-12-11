#include "common.h"

void test() {
  {
    __m128 a = {1.0, 2.0, 3.0, 4.0};
    __m128 b = {5.0, 6.0, 2.0, 1.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vfcmp_slt_s(a, b));
  }

  {
    __m128d a = {1.0, 2.0};
    __m128d b = {3.0, 1.0};
    PRINT(a);
    PRINT(b);
    PRINT(__lsx_vfcmp_slt_d(a, b));
  }
}
