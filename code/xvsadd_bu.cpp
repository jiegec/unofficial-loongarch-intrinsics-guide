#include "common.h"

v256 xvsadd_bu(v256 a, v256 b) {
  v256 dst;
#include "xvsadd_bu.h"
  return dst;
}

void test() { XFUZZ2(xvsadd_bu); }
