#include "common.h"

v128 vsrlri_h(v128 a, int imm) {
  v128 dst;
#include "vsrlri_h.h"
  return dst;
}

void test() {
  FUZZ1(vsrlri_h, 0);
  FUZZ1(vsrlri_h, 7);
}
