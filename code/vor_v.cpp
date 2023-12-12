#include "common.h"

v128 vor_v(v128 a, v128 b) {
  v128 dst;
#include "vor_v.h"
  return dst;
}

void test() { FUZZ2(vor_v); }
