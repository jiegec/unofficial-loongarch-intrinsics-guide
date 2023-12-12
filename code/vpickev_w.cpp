#include "common.h"

v128 vpickev_w(v128 a, v128 b) {
  v128 dst;
#include "vpickev_w.h"
  return dst;
}

void test() { FUZZ2(vpickev_w); }
