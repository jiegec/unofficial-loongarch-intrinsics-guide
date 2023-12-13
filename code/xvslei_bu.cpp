#include "common.h"

v256 xvslei_bu(v256 a, int imm) {
  v256 dst;
#include "xvslei_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvslei_bu, 0);
  XFUZZ1(xvslei_bu, 15);
}
