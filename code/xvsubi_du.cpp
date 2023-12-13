#include "common.h"

v256 xvsubi_du(v256 a, int imm) {
  v256 dst;
#include "xvsubi_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvsubi_du, 0);
  XFUZZ1(xvsubi_du, 31);
}
