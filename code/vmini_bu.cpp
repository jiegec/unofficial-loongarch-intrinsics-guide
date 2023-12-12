#include "common.h"

v128 vmini_bu(v128 a, int imm) {
  v128 dst;
#include "vmini_bu.h"
  return dst;
}

void test() {
  FUZZ1(vmini_bu, 0);
  FUZZ1(vmini_bu, 3);
  FUZZ1(vmini_bu, 15);
}
