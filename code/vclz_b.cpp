#include "common.h"

v128 vclz_b(v128 a) {
  v128 dst;
#include "vclz_b.h"
  return dst;
}

void test() { FUZZ1(vclz_b); }
