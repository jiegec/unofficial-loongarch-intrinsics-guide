#include "common.h"

v256 xvhsubw_w_h(v256 a, v256 b) {
  v256 dst;
#include "xvhsubw_w_h.h"
  return dst;
}

void test() { XFUZZ2(xvhsubw_w_h); }
