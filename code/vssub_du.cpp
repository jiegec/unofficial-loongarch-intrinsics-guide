#include "common.h"

v128 vssub_du(v128 a, v128 b) {
  v128 dst;
#include "vssub_du.h"
  return dst;
}

void test() { FUZZ2(vssub_du); }
