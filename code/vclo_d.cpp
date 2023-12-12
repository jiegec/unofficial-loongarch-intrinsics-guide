#include "common.h"

v128 vclo_d(v128 a) {
  v128 dst;
#include "vclo_d.h"
  return dst;
}

void test() { FUZZ1(vclo_d); }
