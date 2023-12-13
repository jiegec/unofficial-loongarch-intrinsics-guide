#include "common.h"

v256 xvsra_w(v256 a, v256 b) {
  v256 dst;
#include "xvsra_w.h"
  return dst;
}

void test() { XFUZZ2(xvsra_w); }
