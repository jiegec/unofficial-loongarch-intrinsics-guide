#include "common.h"

v128 vexth_h_b(v128 a) {
  v128 dst;
#include "vexth_h_b.h"
  return dst;
}

void test() { FUZZ1(vexth_h_b); }
