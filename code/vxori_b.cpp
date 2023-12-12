#include "common.h"

v128 vxori_b(v128 a, int imm) {
  v128 dst;
#include "vxori_b.h"
  return dst;
}

void test() {
  FUZZ1(vxori_b, 0x00);
  FUZZ1(vxori_b, 0x04);
  FUZZ1(vxori_b, 0x08);
  FUZZ1(vxori_b, 0x10);
  FUZZ1(vxori_b, 0xFF);
}
