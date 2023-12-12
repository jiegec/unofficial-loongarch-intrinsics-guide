#include "common.h"

v128 vbitseti_b(v128 a, int imm) {
  v128 dst;
#include "vbitseti_b.h"
  return dst;
}

void test() {
  FUZZ1(vbitseti_b, 0);
  FUZZ1(vbitseti_b, 3);
  FUZZ1(vbitseti_b, 7);
}
