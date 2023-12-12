#include "common.h"

v128 vavg_w(v128 a, v128 b) {
  v128 dst;
#include "vavg_w.h"
  return dst;
}

void test() {
  FUZZ2(vavg_w);
}
