#include "common.h"

v128 vslei_du(v128 a, int imm) {
  v128 dst;
#include "vslei_du.h"
  return dst;
}

void test() {
  FUZZ1(vslei_du, 0);
  FUZZ1(vslei_du, 15);
}
