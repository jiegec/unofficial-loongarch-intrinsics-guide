#include "common.h"

v256 xvnori_b(v256 a, int imm) {
  v256 dst;
#include "xvnori_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvnori_b, 0);
  XFUZZ1(xvnori_b, 31);
}
