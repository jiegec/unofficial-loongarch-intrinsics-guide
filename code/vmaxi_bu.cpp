#include "common.h"

v128 vmaxi_bu(v128 a, int imm) {
  v128 dst;
#include "vmaxi_bu.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_bu, 0);
  FUZZ1(vmaxi_bu, 3);
  FUZZ1(vmaxi_bu, 15);
}
