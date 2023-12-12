#include "common.h"

v128 vsll_d(v128 a, v128 b) {
  v128 dst;
#include "vsll_d.h"
  return dst;
}

void test() { FUZZ2(vsll_d); }
