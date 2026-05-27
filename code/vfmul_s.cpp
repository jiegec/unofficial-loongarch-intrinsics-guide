#include "common.h"

v128 vfmul_s(v128 a, v128 b) {
  v128 dst;
#include "vfmul_s.h"
  return dst;
}

void test() { FUZZ2(vfmul_s); }
