#include "common.h"

v256 xvsadd_h(v256 a, v256 b) {
  v256 dst;
#include "xvsadd_h.h"
  return dst;
}

void test() { XFUZZ2(xvsadd_h); }
