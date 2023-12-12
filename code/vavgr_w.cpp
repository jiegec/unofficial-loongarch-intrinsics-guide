#include "common.h"

v128 vavgr_w(v128 a, v128 b) {
  v128 dst;
#include "vavgr_w.h"
  return dst;
}

void test() {
  FUZZ2(vavgr_w);
}
