#include "common.h"

v256 xvsub_d(v256 a, v256 b) {
  v256 dst;
#include "xvsub_d.h"
  return dst;
}

void test() { XFUZZ2(xvsub_d); }
