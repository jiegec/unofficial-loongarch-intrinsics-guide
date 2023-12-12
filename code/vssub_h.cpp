#include "common.h"

v128 vssub_h(v128 a, v128 b) {
  v128 dst;
#include "vssub_h.h"
  return dst;
}

void test() { FUZZ2(vssub_h); }
