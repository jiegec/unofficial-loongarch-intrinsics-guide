#include "common.h"

v128 vavg_d(v128 a, v128 b) {
  v128 dst;
#include "vavg_d.h"
  return dst;
}

void test() {
  FUZZ2(vavg_d);
}
