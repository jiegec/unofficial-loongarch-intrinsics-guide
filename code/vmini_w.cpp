#include "common.h"

v128 vmini_w(v128 a, int imm) {
  v128 dst;
#include "vmini_w.h"
  return dst;
}

void test() {
  FUZZ1(vmini_w, 0);
  FUZZ1(vmini_w, 3);
  FUZZ1(vmini_w, 15);
}
