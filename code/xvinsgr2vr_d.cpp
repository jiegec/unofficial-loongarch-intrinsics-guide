#include "common.h"

v256 xvinsgr2vr_d(v256 a, long int b, int imm) {
  v256 dst;
#include "xvinsgr2vr_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvinsgr2vr_d, 0, 0);
  XFUZZ1(xvinsgr2vr_d, 1234, 1);
  XFUZZ1(xvinsgr2vr_d, 5678, 1);
}
