#include "common.h"

v256 xvbitrev_b(v256 a, v256 b) {
  v256 dst;
#include "xvbitrev_b.h"
  return dst;
}

void test() { XFUZZ2(xvbitrev_b); }
