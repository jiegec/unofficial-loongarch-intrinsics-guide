#include "common.h"

v256 xvavgr_w(v256 a, v256 b) {
  v256 dst;
#include "xvavgr_w.h"
  return dst;
}

void test() { XFUZZ2(xvavgr_w); }
