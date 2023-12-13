#include "common.h"

v256 xvslei_b(v256 a, int imm) {
  v256 dst;
#include "xvslei_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvslei_b, 0);
  XFUZZ1(xvslei_b, 15);
}
