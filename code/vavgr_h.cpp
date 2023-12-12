#include "common.h"

v128 vavgr_h(v128 a, v128 b) {
  v128 dst;
#include "vavgr_h.h"
  return dst;
}

void test() {
  FUZZ2(vavgr_h);
}
