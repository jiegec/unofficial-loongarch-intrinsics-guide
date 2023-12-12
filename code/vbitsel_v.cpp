#include "common.h"

v128 vbitsel_v(v128 a, v128 b, v128 c) {
  v128 dst;
  for (int i = 0; i < 2; i++) {
    dst.dword[i] = (c.dword[i] & b.dword[i]) | (~c.dword[i] & a.dword[i]);
  }
  return dst;
}

void test() {
  __m128i a = {0x123456789ABCDEF0, 0x0FEDCBA987654321};
  __m128i b = {0x1122334455667788, 0x1212343456567878};
  __m128i c = {0x0FF0FFFF00000000, 0x0102040810204080};
  PRINT(a);
  PRINT(b);
  PRINT(c);
  PRINT(__lsx_vbitsel_v(a, b, c));

  for (int i = 0; i < 64; i++) {
    v128 a, b, c;
    PRINT(a);
    PRINT(b);
    PRINT(c);
    PRINT(__lsx_vbitsel_v(a, b, c));
    PRINT(vbitsel_v(a, b, c));
    assert(vbitsel_v(a, b, c) == __lsx_vbitsel_v(a, b, c));
  }
}
