#include "common.h"

v128 vslti_h(v128 a, int imm) {
  v128 dst;
#include "vslti_h.h"
  return dst;
}

void test() {
  FUZZ1(vslti_h, 0);
  FUZZ1(vslti_h, 15);
}
