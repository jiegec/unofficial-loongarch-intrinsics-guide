#include "common.h"

v128 vrotr_w(v128 a, v128 b) {
  v128 dst;
#include "vrotr_w.h"
  return dst;
}

void test() { FUZZ2(vrotr_w); }
