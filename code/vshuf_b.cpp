#include "common.h"

v128 vshuf_b(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vshuf_b.h"
  return dst;
}

void test() {
  __m128i a = {0x123456789ABCDEF0, 0x0FEDCBA987654321};
  __m128i b = {0x1122334455667788, 0x1212343456567878};
  __m128i c = {0x0001020304050607, 0x0001020304050607};
  PRINT(a);
  PRINT(b);
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i{0x08090A0B0C0D0E0F, 0x08090A0B0C0D0E0F};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i{0x1011121314151617, 0x1011121314151617};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i{0x18191A1B1C1D1E1F, 0x18191A1B1C1D1E1F};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i{0x2021FFFFFFFFFFFF, 0x2223FFFFFFFFFFFF};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i{0x0706050403020100, 0x1F1E1D1C1B1A1918};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  c = __m128i{0x20406080A0C0E0FF, 0x1030507090B0D0F0};
  PRINT(c);
  PRINT(__lsx_vshuf_b(a, b, c));

  a = __m128i{0x1716151413121110, 0x1F1E1D1C1B1A1918};
  b = __m128i{0x0706050403020100, 0x0F0E0D0C0B0A0908};

  for (int i = 0; i < 64; i++) {
    c = __m128i{i, 0};
    PRINT(c);
    PRINT(__lsx_vshuf_b(a, b, c));
    PRINT(vshuf_b(a, b, c));
  }

  FUZZ3(vshuf_b);
}
