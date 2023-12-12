#include "common.h"

v128 vmini_hu(v128 a, int imm) {
  v128 dst;
#include "vmini_hu.h"
  return dst;
}

void test() {
  FUZZ1(vmini_hu, 0);
  FUZZ1(vmini_hu, 3);
  FUZZ1(vmini_hu, 15);
}
