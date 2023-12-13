#include "common.h"

v256 xvsrln_h_w(v256 a, v256 b) {
  v256 dst;
#include "xvsrln_h_w.h"
  return dst;
}

void test() { XFUZZ2(xvsrln_h_w); }
