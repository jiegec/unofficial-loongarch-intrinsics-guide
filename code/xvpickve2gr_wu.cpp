#include "common.h"

unsigned int xvpickve2gr_wu(v256 a, int idx) {
  unsigned int dst;
#include "xvpickve2gr_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvpickve2gr_wu, 0);
  XFUZZ1(xvpickve2gr_wu, 7);
}
