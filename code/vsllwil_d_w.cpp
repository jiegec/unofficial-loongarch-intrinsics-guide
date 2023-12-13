#include "common.h"

v128 vsllwil_d_w(v128 a, int imm) {
  v128 dst;
#include "vsllwil_d_w.h"
  return dst;
}

void test() {
  FUZZ1(vsllwil_d_w, 0);
  FUZZ1(vsllwil_d_w, 7);
}
