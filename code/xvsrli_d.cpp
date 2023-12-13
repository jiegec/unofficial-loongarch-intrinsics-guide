#include "common.h"

v256 xvsrli_d(v256 a, int imm) {
  v256 dst;
#include "xvsrli_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrli_d, 0);
  XFUZZ1(xvsrli_d, 7);
}
