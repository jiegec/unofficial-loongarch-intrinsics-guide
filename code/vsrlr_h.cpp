#include "common.h"

v128 vsrlr_h(v128 a, v128 b) {
  v128 dst;
#include "vsrlr_h.h"
  return dst;
}

void test() { FUZZ2(vsrlr_h); }
