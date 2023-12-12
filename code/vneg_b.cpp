#include "common.h"

v128 vneg_b(v128 a) {
  v128 dst;
#include "vneg_b.h"
  return dst;
}

void test() { FUZZ1(vneg_b); }
