#include "common.h"

v128 vslei_hu(v128 a, int imm) {
  v128 dst;
#include "vslei_hu.h"
  return dst;
}

void test() {
  FUZZ1(vslei_hu, 0);
  FUZZ1(vslei_hu, 15);
}
