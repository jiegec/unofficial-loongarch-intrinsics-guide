#include "common.h"

v256 xvmini_hu(v256 a, int imm) {
  v256 dst;
#include "xvmini_hu.h"
  return dst;
}

void test() {
  XFUZZ1(xvmini_hu, 0);
  XFUZZ1(xvmini_hu, 3);
  XFUZZ1(xvmini_hu, 15);
}
