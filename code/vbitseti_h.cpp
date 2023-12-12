#include "common.h"

v128 vbitseti_h(v128 a, int imm) {
  v128 dst;
#include "vbitseti_h.h"
  return dst;
}

void test() {
  FUZZ1(vbitseti_h, 0);
  FUZZ1(vbitseti_h, 3);
  FUZZ1(vbitseti_h, 7);
}
