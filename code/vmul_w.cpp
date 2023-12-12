#include "common.h"

v128 vmul_w(v128 a, v128 b) {
  v128 dst;
#include "vmul_w.h"
  return dst;
}

void test() { FUZZ2(vmul_w); }
