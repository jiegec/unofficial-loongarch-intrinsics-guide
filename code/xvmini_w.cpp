#include "common.h"

v256 xvmini_w(v256 a, int imm) {
  v256 dst;
#include "xvmini_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_w, 0);
  XFUZZ1(xvmini_w, 3);
  XFUZZ1(xvmini_w, 15);
}
