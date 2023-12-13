#include "common.h"

v256 xvsadd_wu(v256 a, v256 b) {
  v256 dst;
#include "xvsadd_wu.h"
  return dst;
}

void test() { XFUZZ2(xvsadd_wu); }
