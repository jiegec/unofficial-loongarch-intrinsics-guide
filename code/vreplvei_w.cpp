#include "common.h"

v128 vreplvei_w(v128 a, int idx) {
  v128 dst;
#include "vreplvei_w.h"
  return dst;
}

void test() {
  FUZZ1(vreplvei_w, 0);
  FUZZ1(vreplvei_w, 1);
}
