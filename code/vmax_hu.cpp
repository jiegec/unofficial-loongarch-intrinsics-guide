#include "common.h"

v128 vmax_hu(v128 a, v128 b) {
  v128 dst;
#include "vmax_hu.h"
  return dst;
}

void test() { FUZZ2(vmax_hu); }
