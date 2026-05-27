#include "common.h"

v128 vffint_d_lu(v128 a) {
  v128 dst;
#include "vffint_d_lu.h"
  return dst;
}

void test() { FUZZ1(vffint_d_lu); }
