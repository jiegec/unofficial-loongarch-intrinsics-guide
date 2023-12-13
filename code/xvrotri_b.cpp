#include "common.h"

v256 xvrotri_b(v256 a, int imm) {
  v256 dst;
#include "xvrotri_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvrotri_b, 0);
  XFUZZ1(xvrotri_b, 7);
}
