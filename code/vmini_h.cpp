#include "common.h"

v128 vmini_h(v128 a, int imm) {
  v128 dst;
#include "vmini_h.h"
  return dst;
}

void test() {
  FUZZ1(vmini_h, 0);
  FUZZ1(vmini_h, 3);
  FUZZ1(vmini_h, 15);
}
