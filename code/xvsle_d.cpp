#include "common.h"

v256 xvsle_d(v256 a, v256 b) {
  v256 dst;
#include "xvsle_d.h"
  return dst;
}

void test() { XFUZZ2(xvsle_d); }
