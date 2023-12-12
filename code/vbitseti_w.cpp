#include "common.h"

v128 vbitseti_w(v128 a, int imm) {
  v128 dst;
#include "vbitseti_w.h"
  return dst;
}

void test() {
  FUZZ1(vbitseti_w, 0);
  FUZZ1(vbitseti_w, 3);
  FUZZ1(vbitseti_w, 7);
}
