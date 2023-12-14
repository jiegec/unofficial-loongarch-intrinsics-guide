#include "common.h"

int vpickve2gr_w(v128 a, int idx) {
  int dst;
#include "vpickve2gr_w.h"
  return dst;
}

void test() {
  FUZZ1(vpickve2gr_w, 0);
  FUZZ1(vpickve2gr_w, 3);
}
