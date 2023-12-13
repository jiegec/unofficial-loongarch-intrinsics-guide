#include "common.h"

v256 xvsub_b(v256 a, v256 b) {
  v256 dst;
#include "xvsub_b.h"
  return dst;
}

void test() { XFUZZ2(xvsub_b); }
