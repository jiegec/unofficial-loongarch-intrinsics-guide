#include "common.h"

v256 xvbitset_w(v256 a, v256 b) {
  v256 dst;
#include "xvbitset_w.h"
  return dst;
}

void test() { XFUZZ2(xvbitset_w); }
