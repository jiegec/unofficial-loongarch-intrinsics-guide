#include "common.h"

v128 vpackev_w(v128 a, v128 b) {
  v128 dst;
#include "vpackev_w.h"
  return dst;
}

void test() { FUZZ2(vpackev_w); }
