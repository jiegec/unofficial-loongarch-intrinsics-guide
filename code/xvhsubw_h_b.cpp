#include "common.h"

v256 xvhsubw_h_b(v256 a, v256 b) {
  v256 dst;
#include "xvhsubw_h_b.h"
  return dst;
}

void test() { XFUZZ2(xvhsubw_h_b); }
