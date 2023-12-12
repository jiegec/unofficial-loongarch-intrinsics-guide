#include "common.h"

v128 vpackev_d(v128 a, v128 b) {
  v128 dst;
#include "vpackev_d.h"
  return dst;
}

void test() { FUZZ2(vpackev_d); }
