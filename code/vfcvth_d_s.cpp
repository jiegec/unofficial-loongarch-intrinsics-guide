#include "common.h"

v128 vfcvth_d_s(v128 a) {
  v128 dst;
#include "vfcvth_d_s.h"
  return dst;
}

void test() { FUZZ1(vfcvth_d_s); }
