#include "common.h"

long int vpickve2gr_d(v128 a, int idx) {
  long int dst;
#include "vpickve2gr_d.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_d, 0);
  FUZZ1(vpickve2gr_d, 1);
}
