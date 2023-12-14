#include "common.h"

v256 xvpickve_w(v256 a, int imm) {
  v256 dst;
#include "xvpickve_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvpickve_w, 0);
  XFUZZ1(xvpickve_w, 1);
  XFUZZ1(xvpickve_w, 2);
  XFUZZ1(xvpickve_w, 3);
}
