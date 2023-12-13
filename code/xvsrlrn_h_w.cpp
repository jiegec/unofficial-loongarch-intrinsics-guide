#include "common.h"

v256 xvsrlrn_h_w(v256 a, v256 b) {
  v256 dst;
#include "xvsrlrn_h_w.h"
  return dst;
}

void test() { XFUZZ2(xvsrlrn_h_w); }
