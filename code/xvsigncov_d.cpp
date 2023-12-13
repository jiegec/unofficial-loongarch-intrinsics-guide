#include "common.h"

v256 xvsigncov_d(v256 a, v256 b) {
  v256 dst;
#include "xvsigncov_d.h"
  return dst;
}

void test() { XFUZZ2(xvsigncov_d); }
