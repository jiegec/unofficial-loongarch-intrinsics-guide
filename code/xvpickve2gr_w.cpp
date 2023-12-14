#include "common.h"

int xvpickve2gr_w(v256 a, int idx) {
  int dst;
#include "xvpickve2gr_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvpickve2gr_w, 0);
  XFUZZ1(xvpickve2gr_w, 7);
}
