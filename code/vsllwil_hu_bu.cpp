#include "common.h"

v128 vsllwil_hu_bu(v128 a, int imm) {
  v128 dst;
#include "vsllwil_hu_bu.h"
  return dst;
}

void test() {
  FUZZ1(vsllwil_hu_bu, 0);
  FUZZ1(vsllwil_hu_bu, 7);
}
