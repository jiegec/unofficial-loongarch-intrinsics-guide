#include "common.h"

v256 xvmini_wu(v256 a, int imm) {
  v256 dst;
#include "xvmini_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_wu, 0);
  XFUZZ1(xvmini_wu, 3);
  XFUZZ1(xvmini_wu, 15);
}
