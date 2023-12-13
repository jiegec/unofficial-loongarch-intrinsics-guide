#include "common.h"

v128 vsllwil_du_wu(v128 a, int imm) {
  v128 dst;
#include "vsllwil_du_wu.h"
  return dst;
}

void test() {
  FUZZ1(vsllwil_du_wu, 0);
  FUZZ1(vsllwil_du_wu, 7);
}
