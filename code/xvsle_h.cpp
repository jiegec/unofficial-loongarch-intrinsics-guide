#include "common.h"

v256 xvsle_h(v256 a, v256 b) {
  v256 dst;
#include "xvsle_h.h"
  return dst;
}

void test() { XFUZZ2(xvsle_h); }
