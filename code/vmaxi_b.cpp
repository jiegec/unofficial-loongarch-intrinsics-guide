#include "common.h"

v128 vmaxi_b(v128 a, int imm) {
  v128 dst;
#include "vmaxi_b.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_b, 0);
  FUZZ1(vmaxi_b, 3);
  FUZZ1(vmaxi_b, 15);
}
