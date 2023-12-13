#include "common.h"

v256 xvmulwod_w_hu(v256 a, v256 b) {
  v256 dst;
#include "xvmulwod_w_hu.h"
  return dst;
}

void test() { XFUZZ2(xvmulwod_w_hu); }
