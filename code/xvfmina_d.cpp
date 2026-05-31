#include "common.h"

v256 xvfmina_d(v256 a, v256 b) {
  v256 dst;
#include "xvfmina_d.h"
  return dst;
}

void test() { XFUZZ2(xvfmina_d); }
