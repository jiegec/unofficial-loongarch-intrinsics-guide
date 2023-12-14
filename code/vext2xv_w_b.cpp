#include "common.h"

v256 vext2xv_w_b(v256 a) {
  v256 dst;
#include "vext2xv_w_b.h"
  return dst;
}

void test() { XFUZZ1(vext2xv_w_b); }
