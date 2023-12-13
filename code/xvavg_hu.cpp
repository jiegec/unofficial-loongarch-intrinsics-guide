#include "common.h"

v256 xvavg_hu(v256 a, v256 b) {
  v256 dst;
#include "xvavg_hu.h"
  return dst;
}

void test() { XFUZZ2(xvavg_hu); }
