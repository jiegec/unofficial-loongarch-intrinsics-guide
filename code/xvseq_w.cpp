#include "common.h"

v256 xvseq_w(v256 a, v256 b) {
  v256 dst;
#include "xvseq_w.h"
  return dst;
}

void test() { XFUZZ2(xvseq_w); }
