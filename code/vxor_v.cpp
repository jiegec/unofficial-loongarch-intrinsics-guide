#include "common.h"

v128 vxor_v(v128 a, v128 b) {
  v128 dst;
#include "vxor_v.h"
  return dst;
}

void test() { FUZZ2(vxor_v); }
