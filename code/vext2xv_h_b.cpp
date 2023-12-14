#include "common.h"

v256 vext2xv_h_b(v256 a) {
  v256 dst;
#include "vext2xv_h_b.h"
  return dst;
}

void test() { XFUZZ1(vext2xv_h_b); }
