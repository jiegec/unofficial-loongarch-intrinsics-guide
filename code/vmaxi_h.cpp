#include "common.h"

v128 vmaxi_h(v128 a, int imm) {
  v128 dst;
#include "vmaxi_h.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_h, 0);
  FUZZ1(vmaxi_h, 3);
  FUZZ1(vmaxi_h, 15);
}
