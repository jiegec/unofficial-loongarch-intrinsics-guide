#include "common.h"

v128 vslti_hu(v128 a, int imm) {
  v128 dst;
#include "vslti_hu.h"
  return dst;
}

void test() {
  FUZZ1(vslti_hu, 0);
  FUZZ1(vslti_hu, 15);
}
