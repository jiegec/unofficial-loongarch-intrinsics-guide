#include "common.h"

v256 xvandi_b(v256 a, int imm) {
  v256 dst;
#include "xvandi_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvandi_b, 0);
  XFUZZ1(xvandi_b, 31);
}
