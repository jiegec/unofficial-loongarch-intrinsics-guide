#include "common.h"

v128 vslei_d(v128 a, int imm) {
  v128 dst;
#include "vslei_d.h"
  return dst;
}

void test() {
  FUZZ1(vslei_d, 0);
  FUZZ1(vslei_d, 15);
}
