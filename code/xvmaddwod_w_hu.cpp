#include "common.h"

v256 xvmaddwod_w_hu(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmaddwod_w_hu.h"
  return dst;
}

void test() { XFUZZ3(xvmaddwod_w_hu); }
