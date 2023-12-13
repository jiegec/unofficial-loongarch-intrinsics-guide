#include "common.h"

v256 xvadd_w(v256 a, v256 b) {
  v256 dst;
#include "xvadd_w.h"
  return dst;
}

void test() { XFUZZ2(xvadd_w); }
