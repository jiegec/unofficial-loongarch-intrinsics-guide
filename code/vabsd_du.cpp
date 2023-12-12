#include "common.h"

v128 vabsd_du(v128 a, v128 b) {
  v128 dst;
#include "vabsd_du.h"
  return dst;
}

void test() { FUZZ2(vabsd_du); }
