#include "common.h"

v256 xvsrlr_b(v256 a, v256 b) {
  v256 dst;
#include "xvsrlr_b.h"
  return dst;
}

void test() { XFUZZ2(xvsrlr_b); }
