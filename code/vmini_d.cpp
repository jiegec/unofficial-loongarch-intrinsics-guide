#include "common.h"

v128 vmini_d(v128 a, int imm) {
  v128 dst;
#include "vmini_d.h"
  return dst;
}

void test() {
  FUZZ1(vmini_d, 0);
  FUZZ1(vmini_d, 3);
  FUZZ1(vmini_d, 15);
}
