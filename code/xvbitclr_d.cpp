#include "common.h"

v256 xvbitclr_d(v256 a, v256 b) {
  v256 dst;
#include "xvbitclr_d.h"
  return dst;
}

void test() { XFUZZ2(xvbitclr_d); }
