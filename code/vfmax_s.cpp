#include "common.h"

v128 vfmax_s(v128 a, v128 b) {
  v128 dst;
#include "vfmax_s.h"
  return dst;
}

void test() { FUZZ2(vfmax_s); }
