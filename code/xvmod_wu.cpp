#include "common.h"

v256 xvmod_wu(v256 a, v256 b) {
  v256 dst;
#include "xvmod_wu.h"
  return dst;
}

void test() { XFUZZ2(xvmod_wu); }
