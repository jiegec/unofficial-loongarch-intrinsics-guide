#include "common.h"

v256 xvadd_h(v256 a, v256 b) {
  v256 dst;
#include "xvadd_h.h"
  return dst;
}

void test() { XFUZZ2(xvadd_h); }
