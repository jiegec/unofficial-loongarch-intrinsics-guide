#include "common.h"

v128 vabsd_d(v128 a, v128 b) {
  v128 dst;
#include "vabsd_d.h"
  return dst;
}

void test() { FUZZ2(vabsd_d); }
