#include "common.h"

v256 xvslt_b(v256 a, v256 b) {
  v256 dst;
#include "xvslt_b.h"
  return dst;
}

void test() { XFUZZ2(xvslt_b); }
