#include "common.h"

v256 xvpickve_w_f(v256 a, int imm) {
  v256 dst;
#include "xvpickve_w_f.h"
  return dst;
}

void test() {
  XFUZZ1(xvpickve_w_f, 0);
  XFUZZ1(xvpickve_w_f, 1);
  XFUZZ1(xvpickve_w_f, 2);
  XFUZZ1(xvpickve_w_f, 3);
}
