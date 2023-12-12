#include "common.h"

v128 vreplve_d(v128 a, int idx) {
  v128 dst;
#include "vreplve_d.h"
  return dst;
}

void test() {
  FUZZ1(vreplve_d, 0);
  FUZZ1(vreplve_d, 1);
}
