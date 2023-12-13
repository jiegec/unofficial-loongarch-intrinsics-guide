#include "common.h"

v256 xvmulwod_h_b(v256 a, v256 b) {
  v256 dst;
#include "xvmulwod_h_b.h"
  return dst;
}

void test() { XFUZZ2(xvmulwod_h_b); }
