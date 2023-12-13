#include "common.h"

v256 xvssrani_bu_h(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrani_bu_h.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrani_bu_h, 0);
  XFUZZ2(xvssrani_bu_h, 7);
  XFUZZ2(xvssrani_bu_h, 15);
}
