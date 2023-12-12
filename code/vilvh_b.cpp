#include "common.h"

v128 vilvh_b(v128 a, v128 b) {
  v128 dst;
#include "vilvh_b.h"
  return dst;
}

void test() { FUZZ2(vilvh_b); }
