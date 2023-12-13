#include "common.h"

v256 xvbitrev_d(v256 a, v256 b) {
  v256 dst;
#include "xvbitrev_d.h"
  return dst;
}

void test() { XFUZZ2(xvbitrev_d); }
