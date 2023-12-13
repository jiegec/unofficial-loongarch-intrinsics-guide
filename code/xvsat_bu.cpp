#include "common.h"

v256 xvsat_bu(v256 a, int imm) {
  v256 dst;
#include "xvsat_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsat_bu, 0);
  XFUZZ1(xvsat_bu, 7);
}
