#include "common.h"

v256 xvmadd_d(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmadd_d.h"
  return dst;
}

void test() { XFUZZ3(xvmadd_d); }
