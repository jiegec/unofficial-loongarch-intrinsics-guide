#include "common.h"

v128 vslei_b(v128 a, int imm) {
  v128 dst;
#include "vslei_b.h"
  return dst;
}

void test() {
  FUZZ1(vslei_b, 0);
  FUZZ1(vslei_b, 15);
}
