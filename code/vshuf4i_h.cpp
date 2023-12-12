#include "common.h"

v128 vshuf4i_h(v128 a, int imm) {
  v128 dst;
#include "vshuf4i_h.h"
  return dst;
}

void test() {
  FUZZ1(vshuf4i_h, 0);
  FUZZ1(vshuf4i_h, 13);
  FUZZ1(vshuf4i_h, 100);
  FUZZ1(vshuf4i_h, 128);
  FUZZ1(vshuf4i_h, 255);
}
