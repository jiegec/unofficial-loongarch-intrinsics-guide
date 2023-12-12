#include "common.h"

v128 vexth_d_w(v128 a) {
  v128 dst;
#include "vexth_d_w.h"
  return dst;
}

void test() { FUZZ1(vexth_d_w); }
