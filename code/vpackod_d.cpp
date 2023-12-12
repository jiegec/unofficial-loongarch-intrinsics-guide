#include "common.h"

v128 vpackod_d(v128 a, v128 b) {
  v128 dst;
#include "vpackod_d.h"
  return dst;
}

void test() { FUZZ2(vpackod_d); }
