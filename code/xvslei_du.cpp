#include "common.h"

v256 xvslei_du(v256 a, int imm) {
  v256 dst;
#include "xvslei_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvslei_du, 0);
  XFUZZ1(xvslei_du, 15);
}
