#include "common.h"

v256 xvslt_hu(v256 a, v256 b) {
  v256 dst;
#include "xvslt_hu.h"
  return dst;
}

void test() { XFUZZ2(xvslt_hu); }
