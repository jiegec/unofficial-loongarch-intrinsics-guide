#include "common.h"

v128 vsrli_w(v128 a, int imm) {
  v128 dst;
#include "vsrli_w.h"
  return dst;
}

void test() {
  FUZZ1(vsrli_w, 0);
  FUZZ1(vsrli_w, 7);
}
