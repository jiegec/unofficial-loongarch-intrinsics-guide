#include "common.h"

v128 vmul_h(v128 a, v128 b) {
  v128 dst;
#include "vmul_h.h"
  return dst;
}

void test() { FUZZ2(vmul_h); }
