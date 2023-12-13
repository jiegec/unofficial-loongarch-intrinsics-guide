#include "common.h"

v256 xvmin_w(v256 a, v256 b) {
  v256 dst;
#include "xvmin_w.h"
  return dst;
}

void test() { XFUZZ2(xvmin_w); }
