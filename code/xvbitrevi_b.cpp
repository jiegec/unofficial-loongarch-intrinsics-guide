#include "common.h"

v256 xvbitrevi_b(v256 a, int imm) {
  v256 dst;
#include "xvbitrevi_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitrevi_b, 0);
  XFUZZ1(xvbitrevi_b, 3);
  XFUZZ1(xvbitrevi_b, 7);
}
