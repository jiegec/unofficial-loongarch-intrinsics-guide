#include "common.h"

v256 xvslli_d(v256 a, int imm) {
  v256 dst;
#include "xvslli_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvslli_d, 0);
  XFUZZ1(xvslli_d, 7);
}
