#include "common.h"

unsigned long int xvpickve2gr_du(v256 a, int idx) {
  unsigned long int dst;
#include "xvpickve2gr_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvpickve2gr_du, 0);
  XFUZZ1(xvpickve2gr_du, 3);
}
