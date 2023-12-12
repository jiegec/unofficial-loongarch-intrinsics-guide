#include "common.h"

v128 vslti_bu(v128 a, int imm) {
  v128 dst;
#include "vslti_bu.h"
  return dst;
}

void test() {
  FUZZ1(vslti_bu, 0);
  FUZZ1(vslti_bu, 15);
}
