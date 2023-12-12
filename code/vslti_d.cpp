#include "common.h"

v128 vslti_d(v128 a, int imm) {
  v128 dst;
#include "vslti_d.h"
  return dst;
}

void test() {
  FUZZ1(vslti_d, 0);
  FUZZ1(vslti_d, 15);
}
