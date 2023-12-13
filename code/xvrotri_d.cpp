#include "common.h"

v256 xvrotri_d(v256 a, int imm) {
  v256 dst;
#include "xvrotri_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvrotri_d, 0);
  XFUZZ1(xvrotri_d, 7);
}
