#include "common.h"

v128 vsrai_d(v128 a, int imm) {
  v128 dst;
#include "vsrai_d.h"
  return dst;
}

void test() {
  FUZZ1(vsrai_d, 0);
  FUZZ1(vsrai_d, 7);
}
