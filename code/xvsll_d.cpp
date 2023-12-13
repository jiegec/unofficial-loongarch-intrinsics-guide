#include "common.h"

v256 xvsll_d(v256 a, v256 b) {
  v256 dst;
#include "xvsll_d.h"
  return dst;
}

void test() { XFUZZ2(xvsll_d); }
