#include "common.h"

v128 vfmax_d(v128 a, v128 b) {
  v128 dst;
#include "vfmax_d.h"
  return dst;
}

void test() { FUZZ2(vfmax_d); }
