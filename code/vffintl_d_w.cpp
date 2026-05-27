#include "common.h"

v128 vffintl_d_w(v128 a) {
  v128 dst;
#include "vffintl_d_w.h"
  return dst;
}

void test() { FUZZ1(vffintl_d_w); }
