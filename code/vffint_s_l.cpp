#include "common.h"

v128 vffint_s_l(v128 a, v128 b) {
  v128 dst;
#include "vffint_s_l.h"
  return dst;
}

void test() { FUZZ2(vffint_s_l); }
