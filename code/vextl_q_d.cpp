#include "common.h"

v128 vextl_q_d(v128 a) {
  v128 dst;
#include "vextl_q_d.h"
  return dst;
}

void test() { FUZZ1(vextl_q_d); }
