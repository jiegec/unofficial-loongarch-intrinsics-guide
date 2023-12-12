#include "common.h"

v128 vshuf_d(v128 a, v128 b, v128 c) {
  v128 dst;
  for (int i = 0; i < 2; i++) {
    if ((a.dword[i] % 256) >= 64) {
      // Caveat: observed in 3C5000, but not in QEMU
      dst.dword[i] = 0;
    } else if ((a.dword[i] % 4) < 2) {
      dst.dword[i] = c.dword[a.dword[i] % 2];
    } else {
      dst.dword[i] = b.dword[a.dword[i] % 2];
    }
  }
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
