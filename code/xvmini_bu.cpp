#include "common.h"

v256 xvmini_bu(v256 a, int imm) {
  v256 dst;
#include "xvmini_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_bu, 0);
  XFUZZ1(xvmini_bu, 3);
  XFUZZ1(xvmini_bu, 15);
}
