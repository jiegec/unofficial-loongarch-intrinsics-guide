#include "common.h"

v256 xvbitrevi_d(v256 a, int imm) {
  v256 dst;
#include "xvbitrevi_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitrevi_d, 0);
  XFUZZ1(xvbitrevi_d, 3);
  XFUZZ1(xvbitrevi_d, 7);
}
