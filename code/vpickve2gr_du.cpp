#include "common.h"

unsigned long int vpickve2gr_du(v128 a, int idx) {
  unsigned long int dst;
#include "vpickve2gr_du.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_du, 0);
  FUZZ1(vpickve2gr_du, 1);
}
