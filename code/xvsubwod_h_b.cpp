#include "common.h"

v256 xvsubwod_h_b(v256 a, v256 b) {
  v256 dst;
#include "xvsubwod_h_b.h"
  return dst;
}

void test() { XFUZZ2(xvsubwod_h_b); }
