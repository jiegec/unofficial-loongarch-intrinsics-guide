#include "common.h"

v256 xvsrai_w(v256 a, int imm) {
  v256 dst;
#include "xvsrai_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrai_w, 0);
  XFUZZ1(xvsrai_w, 7);
}
