#include "common.h"

v128 vmulwod_h_bu(v128 a, v128 b) {
  v128 dst;
#include "vmulwod_h_bu.h"
  return dst;
}

void test() { FUZZ2(vmulwod_h_bu); }
