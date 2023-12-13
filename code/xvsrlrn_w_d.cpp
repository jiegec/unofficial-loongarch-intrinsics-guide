#include "common.h"

v256 xvsrlrn_w_d(v256 a, v256 b) {
  v256 dst;
#include "xvsrlrn_w_d.h"
  return dst;
}

void test() { XFUZZ2(xvsrlrn_w_d); }
