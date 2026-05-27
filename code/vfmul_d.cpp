#include "common.h"

v128 vfmul_d(v128 a, v128 b) {
  v128 dst;
#include "vfmul_d.h"
  return dst;
}

void test() { FUZZ2(vfmul_d); }
