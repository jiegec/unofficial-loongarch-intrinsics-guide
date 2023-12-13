#include "common.h"

v256 xvadd_b(v256 a, v256 b) {
  v256 dst;
#include "xvadd_b.h"
  return dst;
}

void test() { XFUZZ2(xvadd_b); }
