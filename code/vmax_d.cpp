#include "common.h"

v128 vmax_d(v128 a, v128 b) {
  v128 dst;
#include "vmax_d.h"
  return dst;
}

void test() { FUZZ2(vmax_d); }
