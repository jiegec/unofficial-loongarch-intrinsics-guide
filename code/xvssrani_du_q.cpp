#include "common.h"

v256 xvssrani_du_q(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrani_du_q.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrani_du_q, 0);
  XFUZZ2(xvssrani_du_q, 7);
  XFUZZ2(xvssrani_du_q, 15);
}
