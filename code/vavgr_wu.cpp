#include "common.h"

v128 vavgr_wu(v128 a, v128 b) {
  v128 dst;
#include "vavgr_wu.h"
  return dst;
}

void test() {
  FUZZ2(vavgr_wu);
}
