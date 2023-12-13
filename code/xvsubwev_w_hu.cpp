#include "common.h"

v256 xvsubwev_w_hu(v256 a, v256 b) {
  v256 dst;
#include "xvsubwev_w_hu.h"
  return dst;
}

void test() { XFUZZ2(xvsubwev_w_hu); }
