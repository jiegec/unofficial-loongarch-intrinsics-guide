#include "common.h"

v128 vmaxi_w(v128 a, int imm) {
  v128 dst;
#include "vmaxi_w.h"
  return dst;
}

void test() {
  FUZZ1(vmaxi_w, 0);
  FUZZ1(vmaxi_w, 3);
  FUZZ1(vmaxi_w, 15);
}
