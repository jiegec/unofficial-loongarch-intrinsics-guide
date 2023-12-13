#include "common.h"

v256 xvsrari_b(v256 a, int imm) {
  v256 dst;
#include "xvsrari_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrari_b, 0);
  XFUZZ1(xvsrari_b, 7);
}
