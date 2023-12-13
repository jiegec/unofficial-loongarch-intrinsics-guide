#include "common.h"

v256 xvmini_d(v256 a, int imm) {
  v256 dst;
#include "xvmini_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_d, 0);
  XFUZZ1(xvmini_d, 3);
  XFUZZ1(xvmini_d, 15);
}
