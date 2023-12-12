#include "common.h"

v128 vbitrev_b(v128 a, v128 b) {
  v128 dst;
#include "vbitrev_b.h"
  return dst;
}

void test() { FUZZ2(vbitrev_b); }
