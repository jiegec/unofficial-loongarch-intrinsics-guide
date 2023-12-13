#include "common.h"

v256 xvmini_b(v256 a, int imm) {
  v256 dst;
#include "xvmini_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_b, 0);
  XFUZZ1(xvmini_b, 3);
  XFUZZ1(xvmini_b, 15);
}
