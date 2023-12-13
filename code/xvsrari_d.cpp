#include "common.h"

v256 xvsrari_d(v256 a, int imm) {
  v256 dst;
#include "xvsrari_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrari_d, 0);
  XFUZZ1(xvsrari_d, 7);
}
