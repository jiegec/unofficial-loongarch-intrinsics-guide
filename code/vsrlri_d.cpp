#include "common.h"

v128 vsrlri_d(v128 a, int imm) {
  v128 dst;
#include "vsrlri_d.h"
  return dst;
}

void test() {
  FUZZ1(vsrlri_d, 0);
  FUZZ1(vsrlri_d, 7);
}
