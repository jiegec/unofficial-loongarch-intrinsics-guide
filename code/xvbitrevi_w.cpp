#include "common.h"

v256 xvbitrevi_w(v256 a, int imm) {
  v256 dst;
#include "xvbitrevi_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitrevi_w, 0);
  XFUZZ1(xvbitrevi_w, 3);
  XFUZZ1(xvbitrevi_w, 7);
}
