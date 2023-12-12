#include "common.h"

v128 vmini_wu(v128 a, int imm) {
  v128 dst;
#include "vmini_wu.h"
  return dst;
}

void test() {
  FUZZ1(vmini_wu, 0);
  FUZZ1(vmini_wu, 3);
  FUZZ1(vmini_wu, 15);
}
