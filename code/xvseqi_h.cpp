#include "common.h"

v256 xvseqi_h(v256 a, int imm) {
  v256 dst;
#include "xvseqi_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvseqi_h, -16);
  XFUZZ1(xvseqi_h, 0);
  XFUZZ1(xvseqi_h, 15);
}
