#include "common.h"

v256 xvssub_b(v256 a, v256 b) {
  v256 dst;
#include "xvssub_b.h"
  return dst;
}

void test() { XFUZZ2(xvssub_b); }
