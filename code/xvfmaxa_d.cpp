#include "common.h"

v256 xvfmaxa_d(v256 a, v256 b) {
  v256 dst;
#include "xvfmaxa_d.h"
  return dst;
}

void test() { XFUZZ2(xvfmaxa_d); }
