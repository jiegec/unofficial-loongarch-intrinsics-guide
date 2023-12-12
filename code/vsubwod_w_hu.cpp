#include "common.h"

v128 vsubwod_w_hu(v128 a, v128 b) {
  v128 dst;
#include "vsubwod_w_hu.h"
  return dst;
}

void test() { FUZZ2(vsubwod_w_hu); }
