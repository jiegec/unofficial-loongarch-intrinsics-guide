#include "common.h"

v256 xvbitclr_w(v256 a, v256 b) {
  v256 dst;
#include "xvbitclr_w.h"
  return dst;
}

void test() { XFUZZ2(xvbitclr_w); }
