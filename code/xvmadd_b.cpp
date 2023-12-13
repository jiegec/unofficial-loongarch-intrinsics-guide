#include "common.h"

v256 xvmadd_b(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmadd_b.h"
  return dst;
}

void test() { XFUZZ3(xvmadd_b); }
