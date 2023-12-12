#include "common.h"

v128 vreplve_h(v128 a, int idx) {
  v128 dst;
#include "vreplve_h.h"
  return dst;
}

void test() {
  FUZZ1(vreplve_h, 0);
  FUZZ1(vreplve_h, 1);
}
