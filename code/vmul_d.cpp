#include "common.h"

v128 vmul_d(v128 a, v128 b) {
  v128 dst;
#include "vmul_d.h"
  return dst;
}

void test() { FUZZ2(vmul_d); }
