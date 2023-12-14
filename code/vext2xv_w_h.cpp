#include "common.h"

v256 vext2xv_w_h(v256 a) {
  v256 dst;
#include "vext2xv_w_h.h"
  return dst;
}

void test() { XFUZZ1(vext2xv_w_h); }
