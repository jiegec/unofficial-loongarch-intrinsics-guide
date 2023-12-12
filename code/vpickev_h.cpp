#include "common.h"

v128 vpickev_h(v128 a, v128 b) {
  v128 dst;
#include "vpickev_h.h"
  return dst;
}

void test() { FUZZ2(vpickev_h); }
