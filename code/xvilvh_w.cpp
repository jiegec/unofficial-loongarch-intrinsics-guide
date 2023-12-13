#include "common.h"

v256 xvilvh_w(v256 a, v256 b) {
  v256 dst;
#include "xvilvh_w.h"
  return dst;
}

void test() { XFUZZ2(xvilvh_w); }
