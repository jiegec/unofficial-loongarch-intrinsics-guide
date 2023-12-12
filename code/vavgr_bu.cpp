#include "common.h"

v128 vavgr_bu(v128 a, v128 b) {
  v128 dst;
#include "vavgr_bu.h"
  return dst;
}

void test() { FUZZ2(vavgr_bu); }
