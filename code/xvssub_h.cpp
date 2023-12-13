#include "common.h"

v256 xvssub_h(v256 a, v256 b) {
  v256 dst;
#include "xvssub_h.h"
  return dst;
}

void test() { XFUZZ2(xvssub_h); }
