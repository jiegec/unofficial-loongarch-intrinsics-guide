#include "common.h"

v128 vslei_h(v128 a, int imm) {
  v128 dst;
#include "vslei_h.h"
  return dst;
}

void test() {
  FUZZ1(vslei_h, 0);
  FUZZ1(vslei_h, 15);
}
