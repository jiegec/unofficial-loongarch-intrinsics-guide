#include "common.h"

v256 xvsubwev_h_b(v256 a, v256 b) {
  v256 dst;
#include "xvsubwev_h_b.h"
  return dst;
}

void test() { XFUZZ2(xvsubwev_h_b); }
