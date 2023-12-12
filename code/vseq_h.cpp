#include "common.h"

v128 vseq_h(v128 a, v128 b) {
  v128 dst;
#include "vseq_h.h"
  return dst;
}

void test() { FUZZ2(vseq_h); }
