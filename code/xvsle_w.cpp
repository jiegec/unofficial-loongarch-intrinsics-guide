#include "common.h"

v256 xvsle_w(v256 a, v256 b) {
  v256 dst;
#include "xvsle_w.h"
  return dst;
}

void test() { XFUZZ2(xvsle_w); }
