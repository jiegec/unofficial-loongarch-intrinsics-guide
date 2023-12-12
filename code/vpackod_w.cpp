#include "common.h"

v128 vpackod_w(v128 a, v128 b) {
  v128 dst;
#include "vpackod_w.h"
  return dst;
}

void test() { FUZZ2(vpackod_w); }
