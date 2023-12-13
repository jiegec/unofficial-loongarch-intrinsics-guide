#include "common.h"

v256 xvsigncov_b(v256 a, v256 b) {
  v256 dst;
#include "xvsigncov_b.h"
  return dst;
}

void test() { XFUZZ2(xvsigncov_b); }
