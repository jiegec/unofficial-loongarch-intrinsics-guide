#include "common.h"

v128 vpickev_b(v128 a, v128 b) {
  v128 dst;
#include "vpickev_b.h"
  return dst;
}

void test() { FUZZ2(vpickev_b); }
