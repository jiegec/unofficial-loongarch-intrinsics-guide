#include "common.h"

v256 xvssrarni_b_h(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrarni_b_h.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrarni_b_h, 0);
  XFUZZ2(xvssrarni_b_h, 7);
  XFUZZ2(xvssrarni_b_h, 15);
}
