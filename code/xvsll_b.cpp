#include "common.h"

v256 xvsll_b(v256 a, v256 b) {
  v256 dst;
#include "xvsll_b.h"
  return dst;
}

void test() { XFUZZ2(xvsll_b); }
