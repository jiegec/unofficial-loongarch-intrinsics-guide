#include "common.h"

v128 vexth_q_d(v128 a) {
  v128 dst;
#include "vexth_q_d.h"
  return dst;
}

void test() { FUZZ1(vexth_q_d); }
