#include "common.h"

v128 vsubwev_h_bu(v128 a, v128 b) {
  v128 dst;
#include "vsubwev_h_bu.h"
  return dst;
}

void test() { FUZZ2(vsubwev_h_bu); }
