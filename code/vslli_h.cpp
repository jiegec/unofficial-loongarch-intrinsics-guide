#include "common.h"

v128 vslli_h(v128 a, int imm) {
  v128 dst;
#include "vslli_h.h"
  return dst;
}

void test() {
  FUZZ1(vslli_h, 0);
  FUZZ1(vslli_h, 7);
}
