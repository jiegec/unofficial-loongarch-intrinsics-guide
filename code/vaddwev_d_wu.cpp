#include "common.h"

v128 vaddwev_d_wu(v128 a, v128 b) {
  v128 dst;
#include "vaddwev_d_wu.h"
  return dst;
}

void test() { FUZZ2(vaddwev_d_wu); }
