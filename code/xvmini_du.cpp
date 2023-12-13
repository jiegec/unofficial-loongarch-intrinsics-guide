#include "common.h"

v256 xvmini_du(v256 a, int imm) {
  v256 dst;
#include "xvmini_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_du, 0);
  XFUZZ1(xvmini_du, 3);
  XFUZZ1(xvmini_du, 15);
}
