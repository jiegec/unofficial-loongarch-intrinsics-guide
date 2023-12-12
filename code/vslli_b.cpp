#include "common.h"

v128 vslli_b(v128 a, int imm) {
  v128 dst;
#include "vslli_b.h"
  return dst;
}

void test() {
  FUZZ1(vslli_b, 0);
  FUZZ1(vslli_b, 7);
}
