#include "common.h"

v256 xvsllwil_hu_bu(v256 a, int imm) {
  v256 dst;
#include "xvsllwil_hu_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsllwil_hu_bu, 0);
  XFUZZ1(xvsllwil_hu_bu, 7);
}
