#include "common.h"

v256 vext2xv_d_w(v256 a) {
  v256 dst;
#include "vext2xv_d_w.h"
  return dst;
}

void test() { XFUZZ1(vext2xv_d_w); }
