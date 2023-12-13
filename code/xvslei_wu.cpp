#include "common.h"

v256 xvslei_wu(v256 a, int imm) {
  v256 dst;
#include "xvslei_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvslei_wu, 0);
  XFUZZ1(xvslei_wu, 15);
}
