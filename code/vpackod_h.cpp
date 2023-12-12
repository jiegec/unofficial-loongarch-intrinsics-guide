#include "common.h"

v128 vpackod_h(v128 a, v128 b) {
  v128 dst;
#include "vpackod_h.h"
  return dst;
}

void test() { FUZZ2(vpackod_h); }
