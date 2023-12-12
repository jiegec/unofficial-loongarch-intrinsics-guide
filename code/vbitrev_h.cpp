#include "common.h"

v128 vbitrev_h(v128 a, v128 b) {
  v128 dst;
#include "vbitrev_h.h"
  return dst;
}

void test() { FUZZ2(vbitrev_h); }
