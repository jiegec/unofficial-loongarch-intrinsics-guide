#include "common.h"

v256 xvsat_du(v256 a, int imm) {
  v256 dst;
#include "xvsat_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvsat_du, 0);
  XFUZZ1(xvsat_du, 7);
}
