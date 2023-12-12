#include "common.h"

v128 vclz_d(v128 a) {
  v128 dst;
#include "vclz_d.h"
  return dst;
}

void test() { FUZZ1(vclz_d); }
