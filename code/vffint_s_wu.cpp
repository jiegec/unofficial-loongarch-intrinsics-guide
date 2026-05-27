#include "common.h"

v128 vffint_s_wu(v128 a) {
  v128 dst;
#include "vffint_s_wu.h"
  return dst;
}

void test() { FUZZ1(vffint_s_wu); }
