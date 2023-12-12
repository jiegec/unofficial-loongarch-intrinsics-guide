#include "common.h"

v128 vavg_h(v128 a, v128 b) {
  v128 dst;
#include "vavg_h.h"
  return dst;
}

void test() { FUZZ2(vavg_h); }
