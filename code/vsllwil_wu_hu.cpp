#include "common.h"

v128 vsllwil_wu_hu(v128 a, int imm) {
  v128 dst;
#include "vsllwil_wu_hu.h"
  return dst;
}

void test() {
  FUZZ1(vsllwil_wu_hu, 0);
  FUZZ1(vsllwil_wu_hu, 7);
}
