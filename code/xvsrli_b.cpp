#include "common.h"

v256 xvsrli_b(v256 a, int imm) {
  v256 dst;
#include "xvsrli_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrli_b, 0);
  XFUZZ1(xvsrli_b, 7);
}
