#include "common.h"

v256 xvssrln_w_d(v256 a, v256 b) {
  v256 dst;
#include "xvssrln_w_d.h"
  return dst;
}

void test() { XFUZZ2(xvssrln_w_d); }
