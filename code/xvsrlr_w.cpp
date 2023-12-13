#include "common.h"

v256 xvsrlr_w(v256 a, v256 b) {
  v256 dst;
#include "xvsrlr_w.h"
  return dst;
}

void test() { XFUZZ2(xvsrlr_w); }
