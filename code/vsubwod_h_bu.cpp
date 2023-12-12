#include "common.h"

v128 vsubwod_h_bu(v128 a, v128 b) {
  v128 dst;
#include "vsubwod_h_bu.h"
  return dst;
}

void test() { FUZZ2(vsubwod_h_bu); }
