#include "common.h"

unsigned int vpickve2gr_bu(v128 a, int idx) {
  unsigned int dst;
#include "vpickve2gr_bu.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_bu, 0);
  FUZZ1(vpickve2gr_bu, 7);
}
