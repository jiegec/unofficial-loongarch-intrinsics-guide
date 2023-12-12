#include "common.h"

v128 vbsrl_v(v128 a, int imm) {
  v128 dst;
#include "vbsrl_v.h"
  return dst;
}

void test() {
  FUZZ1(vbsrl_v, 0);
  FUZZ1(vbsrl_v, 3);
  FUZZ1(vbsrl_v, 7);
  FUZZ1(vbsrl_v, 8);
  FUZZ1(vbsrl_v, 16);
  FUZZ1(vbsrl_v, 24);
  FUZZ1(vbsrl_v, 25);
  FUZZ1(vbsrl_v, 31);
}
