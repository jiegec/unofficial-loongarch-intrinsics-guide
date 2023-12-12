#include "common.h"

v128 vsrai_w(v128 a, int imm) {
  v128 dst;
#include "vsrai_w.h"
  return dst;
}

void test() {
  FUZZ1(vsrai_w, 0);
  FUZZ1(vsrai_w, 7);
}
