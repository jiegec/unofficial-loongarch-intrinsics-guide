#include "common.h"

v256 xvsub_h(v256 a, v256 b) {
  v256 dst;
#include "xvsub_h.h"
  return dst;
}

void test() { XFUZZ2(xvsub_h); }
