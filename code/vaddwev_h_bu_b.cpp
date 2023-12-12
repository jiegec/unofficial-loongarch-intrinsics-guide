#include "common.h"

v128 vaddwev_h_bu_b(v128 a, v128 b) {
  v128 dst;
#include "vaddwev_h_bu_b.h"
  return dst;
}

void test() { FUZZ2(vaddwev_h_bu_b); }
