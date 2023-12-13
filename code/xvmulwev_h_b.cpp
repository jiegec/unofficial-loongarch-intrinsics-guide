#include "common.h"

v256 xvmulwev_h_b(v256 a, v256 b) {
  v256 dst;
#include "xvmulwev_h_b.h"
  return dst;
}

void test() { XFUZZ2(xvmulwev_h_b); }
