#include "common.h"

v256 xvbitset_d(v256 a, v256 b) {
  v256 dst;
#include "xvbitset_d.h"
  return dst;
}

void test() { XFUZZ2(xvbitset_d); }
