#include "common.h"

v256 xvpackev_b(v256 a, v256 b) {
  v256 dst;
#include "xvpackev_b.h"
  return dst;
}

void test() { XFUZZ2(xvpackev_b); }
