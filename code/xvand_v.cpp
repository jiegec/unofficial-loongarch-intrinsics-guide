#include "common.h"

v256 xvand_v(v256 a, v256 b) {
  v256 dst;
#include "xvand_v.h"
  return dst;
}

void test() { XFUZZ2(xvand_v); }
