#include "common.h"

v128 vandn_v(v128 a, v128 b) {
  v128 dst;
#include "vandn_v.h"
  return dst;
}

void test() { FUZZ2(vandn_v); }
