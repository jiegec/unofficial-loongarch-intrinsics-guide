#include "common.h"

v128 vsubwev_d_wu(v128 a, v128 b) {
  v128 dst;
#include "vsubwev_d_wu.h"
  return dst;
}

void test() { FUZZ2(vsubwev_d_wu); }
