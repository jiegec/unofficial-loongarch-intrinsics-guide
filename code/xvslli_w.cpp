#include "common.h"

v256 xvslli_w(v256 a, int imm) {
  v256 dst;
#include "xvslli_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvslli_w, 0);
  XFUZZ1(xvslli_w, 7);
}
