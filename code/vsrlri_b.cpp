#include "common.h"

v128 vsrlri_b(v128 a, int imm) {
  v128 dst;
#include "vsrlri_b.h"
  return dst;
}

void test() {
  FUZZ1(vsrlri_b, 0);
  FUZZ1(vsrlri_b, 7);
}
