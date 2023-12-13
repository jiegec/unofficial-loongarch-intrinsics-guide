#include "common.h"

v256 xvsubwod_w_hu(v256 a, v256 b) {
  v256 dst;
#include "xvsubwod_w_hu.h"
  return dst;
}

void test() { XFUZZ2(xvsubwod_w_hu); }
