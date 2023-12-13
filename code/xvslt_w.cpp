#include "common.h"

v256 xvslt_w(v256 a, v256 b) {
  v256 dst;
#include "xvslt_w.h"
  return dst;
}

void test() { XFUZZ2(xvslt_w); }
