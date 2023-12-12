#include "common.h"

v128 vsle_bu(v128 a, v128 b) {
  v128 dst;
#include "vsle_bu.h"
  return dst;
}

void test() { FUZZ2(vsle_bu); }
