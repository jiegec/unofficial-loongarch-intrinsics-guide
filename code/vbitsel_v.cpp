#include "common.h"

void test() {
  __m128i a = {0x123456789ABCDEF0, 0x0FEDCBA987654321};
  __m128i b = {0x1122334455667788, 0x1212343456567878};
  __m128i c = {0x0FF0FFFF00000000, 0x0102040810204080};
  PRINT(a);
  PRINT(b);
  PRINT(c);
  PRINT(__lsx_vbitsel_v(a, b, c));
}
