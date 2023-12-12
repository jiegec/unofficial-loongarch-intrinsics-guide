#include "common.h"

v128 vneg_h(v128 a) {
  v128 dst;
#include "vneg_h.h"
  return dst;
}

void test() { FUZZ1(vneg_h); }
