#include "common.h"

v128 vbsll_v(v128 a, int imm) {
  v128 dst;
#include "vbsll_v.h"
  return dst;
}

void test() {
  FUZZ1(vbsll_v, 0);
  FUZZ1(vbsll_v, 3);
  FUZZ1(vbsll_v, 7);
  FUZZ1(vbsll_v, 8);
  FUZZ1(vbsll_v, 16);
  FUZZ1(vbsll_v, 24);
  FUZZ1(vbsll_v, 25);
  FUZZ1(vbsll_v, 31);
}
