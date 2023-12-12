#include "common.h"

v128 vsrai_h(v128 a, int imm) {
  v128 dst;
#include "vsrai_h.h"
  return dst;
}

void test() {
  FUZZ1(vsrai_h, 0);
  FUZZ1(vsrai_h, 7);
}
