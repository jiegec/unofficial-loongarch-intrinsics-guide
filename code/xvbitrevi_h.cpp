#include "common.h"

v256 xvbitrevi_h(v256 a, int imm) {
  v256 dst;
#include "xvbitrevi_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitrevi_h, 0);
  XFUZZ1(xvbitrevi_h, 3);
  XFUZZ1(xvbitrevi_h, 7);
}
