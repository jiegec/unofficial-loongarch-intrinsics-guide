#include "common.h"

v256 xvori_b(v256 a, int imm) {
  v256 dst;
#include "xvori_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvori_b, 0);
  XFUZZ1(xvori_b, 31);
}
