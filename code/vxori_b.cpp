#include "common.h"

v128 vxori_b(v128 a, int imm) {
  v128 dst;
#include "vxori_b.h"
  return dst;
}

void test() {
  FUZZ1(vxori_b, 0);
  FUZZ1(vxori_b, 31);
}
