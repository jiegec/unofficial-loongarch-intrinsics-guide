#include "common.h"

v256 xvabsd_bu(v256 a, v256 b) {
  v256 dst;
#include "xvabsd_bu.h"
  return dst;
}

void test() { XFUZZ2(xvabsd_bu); }
