#include "common.h"

v128 vabsd_hu(v128 a, v128 b) {
  v128 dst;
#include "vabsd_hu.h"
  return dst;
}

void test() { FUZZ2(vabsd_hu); }
