#include "common.h"

v128 vreplvei_d(v128 a, int idx) {
  v128 dst;
#include "vreplvei_d.h"
  return dst;
}

void test() {
  FUZZ1(vreplvei_d, 0);
  FUZZ1(vreplvei_d, 1);
}
