#include "common.h"

v128 vbitseli_b(v128 a, v128 b, int imm) {
  v128 dst;
#include "vbitseli_b.h"
  return dst;
}

void test() {
  FUZZ2(vbitseli_b, 0x00);
  FUZZ2(vbitseli_b, 0x01);
  FUZZ2(vbitseli_b, 0x01);
  FUZZ2(vbitseli_b, 0x80);
  FUZZ2(vbitseli_b, 0xFF);
}
