#include "common.h"

v256 xvmin_b(v256 a, v256 b) {
  v256 dst;
#include "xvmin_b.h"
  return dst;
}

void test() { XFUZZ2(xvmin_b); }
