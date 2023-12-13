#include "common.h"

v256 xvsat_d(v256 a, int imm) {
  v256 dst;
#include "xvsat_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvsat_d, 0);
  XFUZZ1(xvsat_d, 7);
}
