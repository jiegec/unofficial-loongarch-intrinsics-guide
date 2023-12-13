#include "common.h"

v256 xvmadd_w(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmadd_w.h"
  return dst;
}

void test() { XFUZZ3(xvmadd_w); }
