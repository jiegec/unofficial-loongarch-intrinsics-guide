#include "common.h"

v256 xvssrln_hu_w(v256 a, v256 b) {
  v256 dst;
#include "xvssrln_hu_w.h"
  return dst;
}

void test() { XFUZZ2(xvssrln_hu_w); }
