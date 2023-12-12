#include "common.h"

v128 vshuf_d(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vshuf_d.h"
  return dst;
}

void test() {
  for (int i = 0; i < 64; i++) {
    v128 a, b, c;
    PRINT(a);
    PRINT(b);
    PRINT(c);
    PRINT(__lsx_vshuf_d(a, b, c));
    PRINT(vshuf_d(a, b, c));
    assert(vshuf_d(a, b, c) == __lsx_vshuf_d(a, b, c));
  }
}
