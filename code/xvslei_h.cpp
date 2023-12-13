#include "common.h"

v256 xvslei_h(v256 a, int imm) {
  v256 dst;
#include "xvslei_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvslei_h, 0);
  XFUZZ1(xvslei_h, 15);
}
