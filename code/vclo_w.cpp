#include "common.h"

v128 vclo_w(v128 a) {
  v128 dst;
#include "vclo_w.h"
  return dst;
}

void test() { FUZZ1(vclo_w); }
