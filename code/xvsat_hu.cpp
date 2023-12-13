#include "common.h"

v256 xvsat_hu(v256 a, int imm) {
  v256 dst;
#include "xvsat_hu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsat_hu, 0);
  XFUZZ1(xvsat_hu, 7);
}
