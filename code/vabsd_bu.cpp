#include "common.h"

v128 vabsd_bu(v128 a, v128 b) {
  v128 dst;
#include "vabsd_bu.h"
  return dst;
}

void test() { FUZZ2(vabsd_bu); }
