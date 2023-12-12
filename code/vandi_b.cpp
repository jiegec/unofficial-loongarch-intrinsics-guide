#include "common.h"

v128 vandi_b(v128 a, int imm) {
  v128 dst;
#include "vandi_b.h"
  return dst;
}

void test() {
  FUZZ1(vandi_b, 0x00);
  FUZZ1(vandi_b, 0x04);
  FUZZ1(vandi_b, 0x08);
  FUZZ1(vandi_b, 0x10);
  FUZZ1(vandi_b, 0xFF);
}
