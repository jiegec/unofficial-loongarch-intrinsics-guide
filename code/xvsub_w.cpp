#include "common.h"

v256 xvsub_w(v256 a, v256 b) {
  v256 dst;
#include "xvsub_w.h"
  return dst;
}

void test() { XFUZZ2(xvsub_w); }
