#include "common.h"

v256 xvavg_d(v256 a, v256 b) {
  v256 dst;
#include "xvavg_d.h"
  return dst;
}

void test() { XFUZZ2(xvavg_d); }
