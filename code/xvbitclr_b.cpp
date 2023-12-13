#include "common.h"

v256 xvbitclr_b(v256 a, v256 b) {
  v256 dst;
#include "xvbitclr_b.h"
  return dst;
}

void test() { XFUZZ2(xvbitclr_b); }
