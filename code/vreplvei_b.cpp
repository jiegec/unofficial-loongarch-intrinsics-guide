#include "common.h"

v128 vreplvei_b(v128 a, int idx) {
  v128 dst;
#include "vreplvei_b.h"
  return dst;
}

void test() {
  FUZZ1(vreplvei_b, 0);
  FUZZ1(vreplvei_b, 1);
}
