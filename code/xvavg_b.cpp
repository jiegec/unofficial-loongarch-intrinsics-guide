#include "common.h"

v256 xvavg_b(v256 a, v256 b) {
  v256 dst;
#include "xvavg_b.h"
  return dst;
}

void test() { XFUZZ2(xvavg_b); }
