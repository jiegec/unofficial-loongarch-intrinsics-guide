#include "common.h"

v128 vpackev_b(v128 a, v128 b) {
  v128 dst;
#include "vpackev_b.h"
  return dst;
}

void test() { FUZZ2(vpackev_b); }
