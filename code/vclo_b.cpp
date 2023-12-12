#include "common.h"

v128 vclo_b(v128 a) {
  v128 dst;
#include "vclo_b.h"
  return dst;
}

void test() { FUZZ1(vclo_b); }
