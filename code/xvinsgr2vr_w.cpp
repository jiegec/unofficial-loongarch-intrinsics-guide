#include "common.h"

v256 xvinsgr2vr_w(v256 a, long int b, int imm) {
  v256 dst;
#include "xvinsgr2vr_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvinsgr2vr_w, 0, 0);
  XFUZZ1(xvinsgr2vr_w, 1234, 1);
  XFUZZ1(xvinsgr2vr_w, 5678, 1);
}
