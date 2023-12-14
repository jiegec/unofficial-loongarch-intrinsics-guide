#include "common.h"

v256 xvsub_q(v256 a, v256 b) {
  v256 dst;
#include "xvsub_q.h"
  return dst;
}

void test() { XFUZZ2(xvsub_q); }
