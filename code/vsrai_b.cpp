#include "common.h"

v128 vsrai_b(v128 a, int imm) {
  v128 dst;
#include "vsrai_b.h"
  return dst;
}

void test() {
  FUZZ1(vsrai_b, 0);
  FUZZ1(vsrai_b, 7);
}
