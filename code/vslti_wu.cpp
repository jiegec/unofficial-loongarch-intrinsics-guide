#include "common.h"

v128 vslti_wu(v128 a, int imm) {
  v128 dst;
#include "vslti_wu.h"
  return dst;
}

void test() {
  FUZZ1(vslti_wu, 0);
  FUZZ1(vslti_wu, 15);
}
