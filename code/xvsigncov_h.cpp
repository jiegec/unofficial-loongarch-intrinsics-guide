#include "common.h"

v256 xvsigncov_h(v256 a, v256 b) {
  v256 dst;
#include "xvsigncov_h.h"
  return dst;
}

void test() { XFUZZ2(xvsigncov_h); }
