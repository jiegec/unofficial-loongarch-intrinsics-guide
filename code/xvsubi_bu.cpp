#include "common.h"

v256 xvsubi_bu(v256 a, int imm) {
  v256 dst;
#include "xvsubi_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsubi_bu, 0);
  XFUZZ1(xvsubi_bu, 31);
}
