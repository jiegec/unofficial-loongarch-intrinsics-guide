#include "common.h"

v256 xvpickve_d(v256 a, int imm) {
  v256 dst;
#include "xvpickve_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvpickve_d, 0);
  XFUZZ1(xvpickve_d, 1);
  XFUZZ1(xvpickve_d, 2);
  XFUZZ1(xvpickve_d, 3);
}
