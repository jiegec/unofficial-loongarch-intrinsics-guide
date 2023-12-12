#include "common.h"

v128 vslli_d(v128 a, int imm) {
  v128 dst;
#include "vslli_d.h"
  return dst;
}

void test() {
  FUZZ1(vslli_d, 0);
  FUZZ1(vslli_d, 7);
}
