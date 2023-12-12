#include "common.h"

v128 vmaxi_wu(v128 a, int imm) {
  v128 dst;
#include "vmaxi_wu.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_wu, 0);
  FUZZ1(vmaxi_wu, 3);
  FUZZ1(vmaxi_wu, 15);
}
