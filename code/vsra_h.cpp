#include "common.h"

v128 vsra_h(v128 a, v128 b) {
  v128 dst;
#include "vsra_h.h"
  return dst;
}

void test() { FUZZ2(vsra_h); }
