#include "common.h"

v128 vhsubw_h_b(v128 a, v128 b) {
  v128 dst;
#include "vhsubw_h_b.h"
  return dst;
}

void test() { FUZZ2(vhsubw_h_b); }
