#include "common.h"

v128 vavgr_du(v128 a, v128 b) {
  v128 dst;
#include "vavgr_du.h"
  return dst;
}

void test() {
  FUZZ2(vavgr_du);
}
