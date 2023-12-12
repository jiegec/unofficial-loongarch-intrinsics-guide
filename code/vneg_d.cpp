#include "common.h"

v128 vneg_d(v128 a) {
  v128 dst;
#include "vneg_d.h"
  return dst;
}

void test() { FUZZ1(vneg_d); }
