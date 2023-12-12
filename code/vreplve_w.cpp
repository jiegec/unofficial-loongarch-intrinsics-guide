#include "common.h"

v128 vreplve_w(v128 a, int idx) {
  v128 dst;
#include "vreplve_w.h"
  return dst;
}

void test() {
  FUZZ1(vreplve_w, 0);
  FUZZ1(vreplve_w, 1);
}
