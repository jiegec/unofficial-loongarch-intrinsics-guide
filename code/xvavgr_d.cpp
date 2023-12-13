#include "common.h"

v256 xvavgr_d(v256 a, v256 b) {
  v256 dst;
#include "xvavgr_d.h"
  return dst;
}

void test() { XFUZZ2(xvavgr_d); }
