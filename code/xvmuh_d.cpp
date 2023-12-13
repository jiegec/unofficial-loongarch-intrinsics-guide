#include "common.h"

v256 xvmuh_d(v256 a, v256 b) {
  v256 dst;
#include "xvmuh_d.h"
  return dst;
}

void test() { XFUZZ2(xvmuh_d); }
