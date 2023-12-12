#include "common.h"

v128 vshuf4i_b(v128 a, int imm) {
  v128 dst;
#include "vshuf4i_b.h"
  return dst;
}

void test() {
  FUZZ1(vshuf4i_b, 0);
  FUZZ1(vshuf4i_b, 13);
  FUZZ1(vshuf4i_b, 100);
  FUZZ1(vshuf4i_b, 128);
  FUZZ1(vshuf4i_b, 255);
}
