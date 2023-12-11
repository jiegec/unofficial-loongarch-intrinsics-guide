#include "common.h"

void test() {
  __m128i a = {0x123456789ABCDEF0, 0x0FEDCBA987654321};
  __m128i b = {0x1122334455667788, 0x1212343456567878};
  __m128i c = {0x0001020304050607, 0x0001020304050607};
  PRINT(a);
  PRINT(b);
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i {0x08090A0B0C0D0E0F, 0x08090A0B0C0D0E0F};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i {0x1011121314151617, 0x1011121314151617};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i {0x18191A1B1C1D1E1F, 0x18191A1B1C1D1E1F};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i {0x2021FFFFFFFFFFFF, 0x2223FFFFFFFFFFFF};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i {0x0706050403020100, 0x1F1E1D1C1B1A1918};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));
}
