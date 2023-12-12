#include "common.h"

v128 vpickod_b(v128 a, v128 b) {
  v128 dst;
#include "vpickod_b.h"
  return dst;
}

void test() { FUZZ2(vpickod_b); }
