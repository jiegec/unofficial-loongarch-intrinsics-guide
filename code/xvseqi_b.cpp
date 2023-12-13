#include "common.h"

v256 xvseqi_b(v256 a, int imm) {
  v256 dst;
#include "xvseqi_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvseqi_b, -16);
  XFUZZ1(xvseqi_b, 0);
  XFUZZ1(xvseqi_b, 15);
}
