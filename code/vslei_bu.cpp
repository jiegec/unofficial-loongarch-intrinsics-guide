#include "common.h"

v128 vslei_bu(v128 a, int imm) {
  v128 dst;
#include "vslei_bu.h"
  return dst;
}

void test() {
  FUZZ1(vslei_bu, 0);
  FUZZ1(vslei_bu, 15);
}
