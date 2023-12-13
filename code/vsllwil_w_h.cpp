#include "common.h"

v128 vsllwil_w_h(v128 a, int imm) {
  v128 dst;
#include "vsllwil_w_h.h"
  return dst;
}

void test() {
  FUZZ1(vsllwil_w_h, 0);
  FUZZ1(vsllwil_w_h, 7);
}
