#include "common.h"

v128 vavg_du(v128 a, v128 b) {
  v128 dst;
#include "vavg_du.h"
  return dst;
}

void test() { FUZZ2(vavg_du); }
