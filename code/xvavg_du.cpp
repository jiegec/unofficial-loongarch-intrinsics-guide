#include "common.h"

v256 xvavg_du(v256 a, v256 b) {
  v256 dst;
#include "xvavg_du.h"
  return dst;
}

void test() { XFUZZ2(xvavg_du); }
