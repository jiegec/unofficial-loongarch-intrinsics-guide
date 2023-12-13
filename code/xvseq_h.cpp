#include "common.h"

v256 xvseq_h(v256 a, v256 b) {
  v256 dst;
#include "xvseq_h.h"
  return dst;
}

void test() { XFUZZ2(xvseq_h); }
