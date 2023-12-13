#include "common.h"

v128 vsllwil_h_b(v128 a, int imm) {
  v128 dst;
#include "vsllwil_h_b.h"
  return dst;
}

void test() {
  FUZZ1(vsllwil_h_b, 0);
  FUZZ1(vsllwil_h_b, 7);
}
