#include "common.h"

v256 xvsle_bu(v256 a, v256 b) {
  v256 dst;
#include "xvsle_bu.h"
  return dst;
}

void test() { XFUZZ2(xvsle_bu); }
