#include "common.h"

v256 xvbitset_b(v256 a, v256 b) {
  v256 dst;
#include "xvbitset_b.h"
  return dst;
}

void test() { XFUZZ2(xvbitset_b); }
