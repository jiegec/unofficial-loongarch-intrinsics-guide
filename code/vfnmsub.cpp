#include "common.h"

void test() {
  {
    __m128 a = {1.0, 2.0, 3.0, 4.0};
    __m128 b = {5.0, 6.0, 7.0, 8.0};
    __m128 c = {9.0, 10.0, 11.0, 12.0};
    PRINT(a);
    PRINT(b);
    PRINT(c);
    PRINT(__lsx_vfnmsub_s(a, b, c));
  }

  {
    __m128d a = {1.0, 2.0};
    __m128d b = {3.0, 4.0};
    __m128d c = {5.0, 6.0};
    PRINT(a);
    PRINT(b);
    PRINT(c);
    PRINT(__lsx_vfnmsub_d(a, b, c));
  }
}
