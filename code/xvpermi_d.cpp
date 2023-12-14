#include "common.h"

v256 xvpermi_d(v256 a, int imm) {
  v256 dst;
#include "xvpermi_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvpermi_d, 0);
  XFUZZ1(xvpermi_d, 1);
  XFUZZ1(xvpermi_d, 2);
  XFUZZ1(xvpermi_d, 0xFF);
}
