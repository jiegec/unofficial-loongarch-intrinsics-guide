#include "common.h"

v256 xvabsd_w(v256 a, v256 b) {
  v256 dst;
#include "xvabsd_w.h"
  return dst;
}

void test() { XFUZZ2(xvabsd_w); }
