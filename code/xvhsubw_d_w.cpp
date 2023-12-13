#include "common.h"

v256 xvhsubw_d_w(v256 a, v256 b) {
  v256 dst;
#include "xvhsubw_d_w.h"
  return dst;
}

void test() { XFUZZ2(xvhsubw_d_w); }
