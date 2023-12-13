#include "common.h"

v256 xvssrarni_du_q(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrarni_du_q.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrarni_du_q, 0);
  XFUZZ2(xvssrarni_du_q, 7);
  XFUZZ2(xvssrarni_du_q, 15);
}
