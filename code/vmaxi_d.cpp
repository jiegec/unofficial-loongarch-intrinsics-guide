#include "common.h"

v128 vmaxi_d(v128 a, int imm) {
  v128 dst;
#include "vmaxi_d.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_d, 0);
  FUZZ1(vmaxi_d, 3);
  FUZZ1(vmaxi_d, 15);
}
