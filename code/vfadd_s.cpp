#include "common.h"

v128 vfadd_s(v128 a, v128 b) {
  v128 dst;
#include "vfadd_s.h"
  return dst;
}

void test() { FUZZ2(vfadd_s); }
