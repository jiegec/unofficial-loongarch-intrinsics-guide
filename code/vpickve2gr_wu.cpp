#include "common.h"

unsigned int vpickve2gr_wu(v128 a, int idx) {
  unsigned int dst;
#include "vpickve2gr_wu.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_wu, 0);
  FUZZ1(vpickve2gr_wu, 3);
}
