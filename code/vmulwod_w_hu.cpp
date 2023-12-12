#include "common.h"

v128 vmulwod_w_hu(v128 a, v128 b) {
  v128 dst;
#include "vmulwod_w_hu.h"
  return dst;
}

void test() { FUZZ2(vmulwod_w_hu); }
