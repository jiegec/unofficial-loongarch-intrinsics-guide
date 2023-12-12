#include "common.h"

v128 vrotri_h(v128 a, int imm) {
  v128 dst;
#include "vrotri_h.h"
  return dst;
}

void test() {
  FUZZ1(vrotri_h, 0);
  FUZZ1(vrotri_h, 7);
}
