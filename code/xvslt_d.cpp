#include "common.h"

v256 xvslt_d(v256 a, v256 b) {
  v256 dst;
#include "xvslt_d.h"
  return dst;
}

void test() { XFUZZ2(xvslt_d); }
