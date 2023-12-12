#include "common.h"

v128 vavgr_b(v128 a, v128 b) {
  v128 dst;
#include "vavgr_b.h"
  return dst;
}

void test() { FUZZ2(vavgr_b); }
