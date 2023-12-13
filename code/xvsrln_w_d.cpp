#include "common.h"

v256 xvsrln_w_d(v256 a, v256 b) {
  v256 dst;
#include "xvsrln_w_d.h"
  return dst;
}

void test() { XFUZZ2(xvsrln_w_d); }
