#include "common.h"

v128 vhaddw_w_h(v128 a, v128 b) {
  v128 dst;
#include "vhaddw_w_h.h"
  return dst;
}

void test() { FUZZ2(vhaddw_w_h); }
