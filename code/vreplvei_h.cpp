#include "common.h"

v128 vreplvei_h(v128 a, int idx) {
  v128 dst;
#include "vreplvei_h.h"
  return dst;
}

void test() {
  FUZZ1(vreplvei_h, 0);
  FUZZ1(vreplvei_h, 1);
}
