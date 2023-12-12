#include "common.h"

v128 vmulwod_d_wu(v128 a, v128 b) {
  v128 dst;
#include "vmulwod_d_wu.h"
  return dst;
}

void test() { FUZZ2(vmulwod_d_wu); }
