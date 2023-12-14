#include "common.h"

v256 xvabsd_h(v256 a, v256 b) {
  v256 dst;
#include "xvabsd_h.h"
  return dst;
}

void test() { XFUZZ2(xvabsd_h); }
