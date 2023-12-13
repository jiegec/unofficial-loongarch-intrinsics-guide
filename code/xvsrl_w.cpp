#include "common.h"

v256 xvsrl_w(v256 a, v256 b) {
  v256 dst;
#include "xvsrl_w.h"
  return dst;
}

void test() { XFUZZ2(xvsrl_w); }
