#include "common.h"

v128 vabsd_w(v128 a, v128 b) {
  v128 dst;
#include "vabsd_w.h"
  return dst;
}

void test() { FUZZ2(vabsd_w); }
