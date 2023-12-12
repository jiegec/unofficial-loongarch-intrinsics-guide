#include "common.h"

v128 vslti_du(v128 a, int imm) {
  v128 dst;
#include "vslti_du.h"
  return dst;
}

void test() {
  FUZZ1(vslti_du, 0);
  FUZZ1(vslti_du, 15);
}
