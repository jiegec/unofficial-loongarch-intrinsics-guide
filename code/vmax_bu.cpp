#include "common.h"

v128 vmax_bu(v128 a, v128 b) {
  v128 dst;
#include "vmax_bu.h"
  return dst;
}

void test() { FUZZ2(vmax_bu); }
