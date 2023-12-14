#include "common.h"

unsigned int vpickve2gr_hu(v128 a, int idx) {
  unsigned int dst;
#include "vpickve2gr_hu.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_hu, 0);
  FUZZ1(vpickve2gr_hu, 7);
}
