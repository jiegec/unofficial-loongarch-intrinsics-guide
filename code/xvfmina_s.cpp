#include "common.h"

v256 xvfmina_s(v256 a, v256 b) {
  v256 dst;
#include "xvfmina_s.h"
  return dst;
}

void test() { XFUZZ2(xvfmina_s); }
