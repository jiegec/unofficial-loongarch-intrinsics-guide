#include "common.h"

v128 vbitseti_d(v128 a, int imm) {
  v128 dst;
#include "vbitseti_d.h"
  return dst;
}

void test() {
  FUZZ1(vbitseti_d, 0);
  FUZZ1(vbitseti_d, 3);
  FUZZ1(vbitseti_d, 7);
}
