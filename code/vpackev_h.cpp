#include "common.h"

v128 vpackev_h(v128 a, v128 b) {
  v128 dst;
#include "vpackev_h.h"
  return dst;
}

void test() { FUZZ2(vpackev_h); }
