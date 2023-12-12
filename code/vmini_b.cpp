#include "common.h"

v128 vmini_b(v128 a, int imm) {
  v128 dst;
#include "vmini_b.h"
  return dst;
}

void test() {
  FUZZ1(vmini_b, 0);
  FUZZ1(vmini_b, 3);
  FUZZ1(vmini_b, 15);
}
