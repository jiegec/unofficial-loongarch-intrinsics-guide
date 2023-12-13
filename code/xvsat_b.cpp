#include "common.h"

v256 xvsat_b(v256 a, int imm) {
  v256 dst;
#include "xvsat_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvsat_b, 0);
  XFUZZ1(xvsat_b, 7);
}
