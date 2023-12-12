#include "common.h"

v128 vslti_w(v128 a, int imm) {
  v128 dst;
#include "vslti_w.h"
  return dst;
}

void test() {
  FUZZ1(vslti_w, 0);
  FUZZ1(vslti_w, 15);
}
