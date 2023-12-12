#include "common.h"

v128 vclz_w(v128 a) {
  v128 dst;
#include "vclz_w.h"
  return dst;
}

void test() { FUZZ1(vclz_w); }
