#include "common.h"

v128 vilvh_h(v128 a, v128 b) {
  v128 dst;
#include "vilvh_h.h"
  return dst;
}

void test() { FUZZ2(vilvh_h); }
