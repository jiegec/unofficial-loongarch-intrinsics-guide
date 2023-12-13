#include "common.h"

v256 xvsrli_w(v256 a, int imm) {
  v256 dst;
#include "xvsrli_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrli_w, 0);
  XFUZZ1(xvsrli_w, 7);
}
