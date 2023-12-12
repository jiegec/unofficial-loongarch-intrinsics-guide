#include "common.h"

v128 vclo_h(v128 a) {
  v128 dst;
#include "vclo_h.h"
  return dst;
}

void test() { FUZZ1(vclo_h); }
