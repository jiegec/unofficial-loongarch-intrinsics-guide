#include "common.h"

v256 xvavg_w(v256 a, v256 b) {
  v256 dst;
#include "xvavg_w.h"
  return dst;
}

void test() { XFUZZ2(xvavg_w); }
