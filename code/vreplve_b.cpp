#include "common.h"

v128 vreplve_b(v128 a, int idx) {
  v128 dst;
#include "vreplve_b.h"
  return dst;
}

void test() {
  FUZZ1(vreplve_b, 0);
  FUZZ1(vreplve_b, 1);
}
