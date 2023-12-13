#include "common.h"

v256 xvmulwod_d_w(v256 a, v256 b) {
  v256 dst;
#include "xvmulwod_d_w.h"
  return dst;
}

void test() { XFUZZ2(xvmulwod_d_w); }
