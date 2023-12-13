#include "common.h"

v256 xvbitrev_w(v256 a, v256 b) {
  v256 dst;
#include "xvbitrev_w.h"
  return dst;
}

void test() { XFUZZ2(xvbitrev_w); }
