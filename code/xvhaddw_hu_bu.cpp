#include "common.h"

v256 xvhaddw_hu_bu(v256 a, v256 b) {
  v256 dst;
#include "xvhaddw_hu_bu.h"
  return dst;
}

void test() { XFUZZ2(xvhaddw_hu_bu); }
