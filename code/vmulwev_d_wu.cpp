#include "common.h"

v128 vmulwev_d_wu(v128 a, v128 b) {
  v128 dst;
#include "vmulwev_d_wu.h"
  return dst;
}

void test() { FUZZ2(vmulwev_d_wu); }
