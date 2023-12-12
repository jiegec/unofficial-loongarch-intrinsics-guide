#include "common.h"

v128 vadda_b(v128 a, v128 b) {
  v128 dst;
#include "vadda_b.h"
  return dst;
}

void test() { FUZZ2(vadda_b); }
