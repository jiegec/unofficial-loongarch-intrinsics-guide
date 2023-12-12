#include "common.h"

v128 vsubwev_w_hu(v128 a, v128 b) {
  v128 dst;
#include "vsubwev_w_hu.h"
  return dst;
}

void test() { FUZZ2(vsubwev_w_hu); }
