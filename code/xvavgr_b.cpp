#include "common.h"

v256 xvavgr_b(v256 a, v256 b) {
  v256 dst;
#include "xvavgr_b.h"
  return dst;
}

void test() { XFUZZ2(xvavgr_b); }
