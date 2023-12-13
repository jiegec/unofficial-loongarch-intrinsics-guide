#include "common.h"

v256 xvmadd_h(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmadd_h.h"
  return dst;
}

void test() { XFUZZ3(xvmadd_h); }
