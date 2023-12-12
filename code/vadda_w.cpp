#include "common.h"

v128 vadda_w(v128 a, v128 b) {
  v128 dst;
#include "vadda_w.h"
  return dst;
}

void test() { FUZZ2(vadda_w); }
