#include "common.h"

v128 vbitrev_d(v128 a, v128 b) {
  v128 dst;
#include "vbitrev_d.h"
  return dst;
}

void test() { FUZZ2(vbitrev_d); }
