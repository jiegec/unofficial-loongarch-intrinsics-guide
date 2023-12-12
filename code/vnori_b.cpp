#include "common.h"

v128 vnori_b(v128 a, int imm) {
  v128 dst;
#include "vnori_b.h"
  return dst;
}

void test() {
  FUZZ1(vnori_b, 0x00);
  FUZZ1(vnori_b, 0x04);
  FUZZ1(vnori_b, 0x08);
  FUZZ1(vnori_b, 0x10);
  FUZZ1(vnori_b, 0xFF);
}
