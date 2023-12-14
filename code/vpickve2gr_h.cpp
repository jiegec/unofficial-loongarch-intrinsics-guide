#include "common.h"

int vpickve2gr_h(v128 a, int idx) {
  int dst;
#include "vpickve2gr_h.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_h, 0);
  FUZZ1(vpickve2gr_h, 7);
}
