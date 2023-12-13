#include "common.h"

v256 xvsll_h(v256 a, v256 b) {
  v256 dst;
#include "xvsll_h.h"
  return dst;
}

void test() { XFUZZ2(xvsll_h); }
