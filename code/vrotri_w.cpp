#include "common.h"

v128 vrotri_w(v128 a, int imm) {
  v128 dst;
#include "vrotri_w.h"
  return dst;
}

void test() {
  FUZZ1(vrotri_w, 0);
  FUZZ1(vrotri_w, 7);
}
