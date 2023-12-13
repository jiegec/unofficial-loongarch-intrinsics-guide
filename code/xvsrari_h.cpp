#include "common.h"

v256 xvsrari_h(v256 a, int imm) {
  v256 dst;
#include "xvsrari_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrari_h, 0);
  XFUZZ1(xvsrari_h, 7);
}
