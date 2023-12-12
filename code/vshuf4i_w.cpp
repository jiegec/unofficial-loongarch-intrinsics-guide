#include "common.h"

v128 vshuf4i_w(v128 a, int imm) {
  v128 dst;
#include "vshuf4i_w.h"
  return dst;
}

void test() {
  FUZZ1(vshuf4i_w, 0);
  FUZZ1(vshuf4i_w, 13);
  FUZZ1(vshuf4i_w, 100);
  FUZZ1(vshuf4i_w, 128);
  FUZZ1(vshuf4i_w, 255);
}
