#include "common.h"

v128 vfcvtl_d_s(v128 a) {
  v128 dst;
#include "vfcvtl_d_s.h"
  return dst;
}

void test() { FUZZ1(vfcvtl_d_s); }
