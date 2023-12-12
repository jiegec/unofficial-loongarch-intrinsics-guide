#include "common.h"

v128 vseq_d(v128 a, v128 b) {
  v128 dst;
#include "vseq_d.h"
  return dst;
}

void test() { FUZZ2(vseq_d); }
