#include "common.h"

v256 xvslei_w(v256 a, int imm) {
  v256 dst;
#include "xvslei_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvslei_w, 0);
  XFUZZ1(xvslei_w, 15);
}
