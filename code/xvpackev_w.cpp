#include "common.h"

v256 xvpackev_w(v256 a, v256 b) {
  v256 dst;
#include "xvpackev_w.h"
  return dst;
}

void test() { XFUZZ2(xvpackev_w); }
