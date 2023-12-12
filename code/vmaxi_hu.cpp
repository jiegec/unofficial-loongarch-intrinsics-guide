#include "common.h"

v128 vmaxi_hu(v128 a, int imm) {
  v128 dst;
#include "vmaxi_hu.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_hu, 0);
  FUZZ1(vmaxi_hu, 3);
  FUZZ1(vmaxi_hu, 15);
}
