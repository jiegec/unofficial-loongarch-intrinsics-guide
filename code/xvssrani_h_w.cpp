#include "common.h"

v256 xvssrani_h_w(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrani_h_w.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrani_h_w, 0);
  XFUZZ2(xvssrani_h_w, 7);
  XFUZZ2(xvssrani_h_w, 15);
}
