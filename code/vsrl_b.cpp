#include "common.h"

v128 vsrl_b(v128 a, v128 b) {
  v128 dst;
#include "vsrl_b.h"
  return dst;
}

void test() { FUZZ2(vsrl_b); }
