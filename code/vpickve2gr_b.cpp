#include "common.h"

int vpickve2gr_b(v128 a, int idx) {
  int dst;
#include "vpickve2gr_b.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_b, 0);
  FUZZ1(vpickve2gr_b, 7);
}
