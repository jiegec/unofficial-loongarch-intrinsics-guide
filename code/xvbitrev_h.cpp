#include "common.h"

v256 xvbitrev_h(v256 a, v256 b) {
  v256 dst;
#include "xvbitrev_h.h"
  return dst;
}

void test() { XFUZZ2(xvbitrev_h); }
