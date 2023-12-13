#include "common.h"

v256 xvsat_wu(v256 a, int imm) {
  v256 dst;
#include "xvsat_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsat_wu, 0);
  XFUZZ1(xvsat_wu, 7);
}
