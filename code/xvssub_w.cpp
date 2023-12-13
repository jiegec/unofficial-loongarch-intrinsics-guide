#include "common.h"

v256 xvssub_w(v256 a, v256 b) {
  v256 dst;
#include "xvssub_w.h"
  return dst;
}

void test() { XFUZZ2(xvssub_w); }
