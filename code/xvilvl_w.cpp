#include "common.h"

v256 xvilvl_w(v256 a, v256 b) {
  v256 dst;
#include "xvilvl_w.h"
  return dst;
}

void test() { XFUZZ2(xvilvl_w); }
