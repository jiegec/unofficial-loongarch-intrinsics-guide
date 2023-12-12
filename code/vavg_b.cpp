#include "common.h"

v128 vavg_b(v128 a, v128 b) {
  v128 dst;
#include "vavg_b.h"
  return dst;
}

void test() { FUZZ2(vavg_b); }
