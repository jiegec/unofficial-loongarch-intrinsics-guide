#include "common.h"

v256 xvsrli_h(v256 a, int imm) {
  v256 dst;
#include "xvsrli_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrli_h, 0);
  XFUZZ1(xvsrli_h, 7);
}
