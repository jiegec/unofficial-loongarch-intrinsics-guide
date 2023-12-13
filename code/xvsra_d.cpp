#include "common.h"

v256 xvsra_d(v256 a, v256 b) {
  v256 dst;
#include "xvsra_d.h"
  return dst;
}

void test() { XFUZZ2(xvsra_d); }
