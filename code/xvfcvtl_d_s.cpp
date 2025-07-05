#include "common.h"

v256 xvfcvtl_d_s(v256 a) {
  v256 dst;
#include "xvfcvtl_d_s.h"
  return dst;
}

void test() { XFUZZ1(xvfcvtl_d_s); }
