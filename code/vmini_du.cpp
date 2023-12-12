#include "common.h"

v128 vmini_du(v128 a, int imm) {
  v128 dst;
#include "vmini_du.h"
  return dst;
}

void test() {
  FUZZ1(vmini_du, 0);
  FUZZ1(vmini_du, 3);
  FUZZ1(vmini_du, 15);
}
