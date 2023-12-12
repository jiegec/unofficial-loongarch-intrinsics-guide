#include "common.h"

v128 vmaxi_du(v128 a, int imm) {
  v128 dst;
#include "vmaxi_du.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_du, 0);
  FUZZ1(vmaxi_du, 3);
  FUZZ1(vmaxi_du, 15);
}
