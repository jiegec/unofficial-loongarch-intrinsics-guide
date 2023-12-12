#include "common.h"

v128 vavg_hu(v128 a, v128 b) {
  v128 dst;
#include "vavg_hu.h"
  return dst;
}

void test() {
  FUZZ2(vavg_hu);
}
