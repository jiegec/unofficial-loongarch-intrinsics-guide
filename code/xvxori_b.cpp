#include "common.h"

v256 xvxori_b(v256 a, int imm) {
  v256 dst;
#include "xvxori_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvxori_b, 0);
  XFUZZ1(xvxori_b, 31);
}
