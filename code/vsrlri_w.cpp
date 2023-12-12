#include "common.h"

v128 vsrlri_w(v128 a, int imm) {
  v128 dst;
#include "vsrlri_w.h"
  return dst;
}

void test() {
  FUZZ1(vsrlri_w, 0);
  FUZZ1(vsrlri_w, 7);
}
