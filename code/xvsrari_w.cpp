#include "common.h"

v256 xvsrari_w(v256 a, int imm) {
  v256 dst;
#include "xvsrari_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrari_w, 0);
  XFUZZ1(xvsrari_w, 7);
}
