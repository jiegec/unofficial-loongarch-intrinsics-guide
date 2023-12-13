#include "common.h"

v256 xvseq_d(v256 a, v256 b) {
  v256 dst;
#include "xvseq_d.h"
  return dst;
}

void test() { XFUZZ2(xvseq_d); }
