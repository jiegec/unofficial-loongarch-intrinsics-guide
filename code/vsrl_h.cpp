#include "common.h"

v128 vsrl_h(v128 a, v128 b) {
  v128 dst;
#include "vsrl_h.h"
  return dst;
}

void test() { FUZZ2(vsrl_h); }
