#include "common.h"

v256 xvseqi_w(v256 a, int imm) {
  v256 dst;
#include "xvseqi_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvseqi_w, -16);
  XFUZZ1(xvseqi_w, 0);
  XFUZZ1(xvseqi_w, 15);
}
