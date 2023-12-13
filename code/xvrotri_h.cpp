#include "common.h"

v256 xvrotri_h(v256 a, int imm) {
  v256 dst;
#include "xvrotri_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvrotri_h, 0);
  XFUZZ1(xvrotri_h, 7);
}
