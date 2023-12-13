#include "common.h"

v256 xvmini_h(v256 a, int imm) {
  v256 dst;
#include "xvmini_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_h, 0);
  XFUZZ1(xvmini_h, 3);
  XFUZZ1(xvmini_h, 15);
}
