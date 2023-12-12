#include "common.h"

v128 vneg_w(v128 a) {
  v128 dst;
#include "vneg_w.h"
  return dst;
}

void test() { FUZZ1(vneg_w); }
