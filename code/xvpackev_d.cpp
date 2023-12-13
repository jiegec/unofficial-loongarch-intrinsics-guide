#include "common.h"

v256 xvpackev_d(v256 a, v256 b) {
  v256 dst;
#include "xvpackev_d.h"
  return dst;
}

void test() { XFUZZ2(xvpackev_d); }
