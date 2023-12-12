#include "common.h"

v128 vslti_b(v128 a, int imm) {
  v128 dst;
#include "vslti_b.h"
  return dst;
}

void test() {
  FUZZ1(vslti_b, 0);
  FUZZ1(vslti_b, 15);
}
