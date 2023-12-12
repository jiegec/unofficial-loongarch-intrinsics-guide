#include "common.h"

v128 vclz_h(v128 a) {
  v128 dst;
#include "vclz_h.h"
  return dst;
}

void test() { FUZZ1(vclz_h); }
