#include "common.h"

v128 vbitrev_w(v128 a, v128 b) {
  v128 dst;
#include "vbitrev_w.h"
  return dst;
}

void test() { FUZZ2(vbitrev_w); }
