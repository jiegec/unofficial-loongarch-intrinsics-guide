#include "common.h"

v256 xvhaddw_w_h(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_w_h.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_w_h); }
