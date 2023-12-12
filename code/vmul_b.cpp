#include "common.h"

v128 vmul_b(v128 a, v128 b) {
  v128 dst;
#include "vmul_b.h"
  return dst;
}

void test() { FUZZ2(vmul_b); }
