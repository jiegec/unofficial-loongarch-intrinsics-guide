#include "common.h"

v128 vpackod_b(v128 a, v128 b) {
  v128 dst;
#include "vpackod_b.h"
  return dst;
}

void test() { FUZZ2(vpackod_b); }
