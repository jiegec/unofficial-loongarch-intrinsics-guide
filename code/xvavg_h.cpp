#include "common.h"

v256 xvavg_h(v256 a, v256 b) {
  v256 dst;
#include "xvavg_h.h"
  return dst;
}

void test() { XFUZZ2(xvavg_h); }
