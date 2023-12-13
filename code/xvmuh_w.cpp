#include "common.h"

v256 xvmuh_w(v256 a, v256 b) {
  v256 dst;
#include "xvmuh_w.h"
  return dst;
}

void test() { XFUZZ2(xvmuh_w); }
