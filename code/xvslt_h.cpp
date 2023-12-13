#include "common.h"

v256 xvslt_h(v256 a, v256 b) {
  v256 dst;
#include "xvslt_h.h"
  return dst;
}

void test() { XFUZZ2(xvslt_h); }
