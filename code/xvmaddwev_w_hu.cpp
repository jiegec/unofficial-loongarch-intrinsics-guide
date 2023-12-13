#include "common.h"

v256 xvmaddwev_w_hu(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmaddwev_w_hu.h"
  return dst;
}

void test() { XFUZZ3(xvmaddwev_w_hu); }
