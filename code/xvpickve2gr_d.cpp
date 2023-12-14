#include "common.h"

long int xvpickve2gr_d(v256 a, int idx) {
  long int dst;
#include "xvpickve2gr_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvpickve2gr_d, 0);
  XFUZZ1(xvpickve2gr_d, 3);
}
