#include "common.h"

v128 vavgr_hu(v128 a, v128 b) {
  v128 dst;
#include "vavgr_hu.h"
  return dst;
}

void test() { FUZZ2(vavgr_hu); }
