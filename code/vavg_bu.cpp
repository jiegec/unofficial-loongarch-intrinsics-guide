#include "common.h"

v128 vavg_bu(v128 a, v128 b) {
  v128 dst;
#include "vavg_bu.h"
  return dst;
}

void test() {
  FUZZ2(vavg_bu);
}
