#include "common.h"

v256 xvfmaxa_s(v256 a, v256 b) {
  v256 dst;
#include "xvfmaxa_s.h"
  return dst;
}

void test() { XFUZZ2(xvfmaxa_s); }
