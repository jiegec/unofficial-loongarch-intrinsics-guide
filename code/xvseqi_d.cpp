#include "common.h"

v256 xvseqi_d(v256 a, int imm) {
  v256 dst;
#include "xvseqi_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvseqi_d, -16);
  XFUZZ1(xvseqi_d, 0);
  XFUZZ1(xvseqi_d, 15);
}
