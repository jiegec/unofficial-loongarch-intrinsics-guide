#include "common.h"

v256 xvssrani_w_d(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrani_w_d.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrani_w_d, 0);
  XFUZZ2(xvssrani_w_d, 7);
  XFUZZ2(xvssrani_w_d, 15);
}
